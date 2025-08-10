#include "Connection.h"

#include <absl/log/log.h>
#include <cryptopp/filters.h>

#include <iostream>
#include <utility>

#include "../BufferCrypter.h"
#include "configuration/clientbound/DisconnectPacket.h"
#include "login/clientbound/LoginDisconnectPacket.h"
#include "src/BufferCompressor.h"
#include "src/MinecraftServer.h"
#include "src/VarInt.h"

namespace celerity::net {
constexpr uint16_t kReadBufferSize = 8192;

void Connection::start() { do_read(); }

void Connection::do_read() {
  LOG(INFO) << "do_read called";
  std::vector<uint8_t> read_buffer;
  read_buffer.resize(kReadBufferSize);

  m_socket.async_read_some(boost::asio::buffer(read_buffer),
                           [self = shared_from_this(), read_buffer = std::move(read_buffer)](
                               const boost::system::error_code& err, const std::size_t bytes_read) mutable {
                             if (err) {
                               if (err == boost::asio::error::operation_aborted) return;
                               if (err != boost::asio::error::eof) {
                                 LOG(ERROR) << "Failed to read socket: " + err.message();
                               }
                               self->close();
                               return;
                             }

                             LOG(INFO) << "async_read_some cb, " << bytes_read << " bytes read";

                             read_buffer.resize(bytes_read);

                             ByteBuffer tmp(std::move(read_buffer));
                             if (self->is_encryption_enabled()) {
                               self->get_buffer_crypter().decrypt(tmp);
                             }

                             self->m_data_buffer.append(tmp);
                             LOG(INFO) << "data appended to data buffer; data buffer has "
                                       << self->m_data_buffer.get_data_length() << " bytes";

                             self->process_buffer();
                             self->do_read();
                           });
}

std::optional<ByteBuffer> Connection::try_pop_packet() {
  LOG(INFO) << "try_pop_packet called";
  const auto maybe_packet_length = m_data_buffer.peek_varint();
  if (!maybe_packet_length) {
    LOG(INFO) << "tpp: not enough data for packet length";
    return std::nullopt;
  }

  const auto [packet_length, packet_length_num_bytes] = *maybe_packet_length;
  if (packet_length <= 0) {
    LOG(WARNING) << "tpp: got packet length <= 0, will not proceed";
    return std::nullopt;
  }

  if (m_data_buffer.get_data_length() - packet_length_num_bytes < packet_length) {
    LOG(INFO) << "tpp: able to read packet length but not enough data to read full packet";
    return std::nullopt;
  }

  LOG(INFO) << "tpp: ok popping front varint";
  m_data_buffer.truncate_front(packet_length_num_bytes);

  size_t payload_bytes = packet_length;
  bool should_decompress = false;

  LOG(INFO) << "Got packet payload bytes: " << payload_bytes;

  if (is_compression_enabled()) {
    uint8_t encoding_length;
    const auto data_length = m_data_buffer.read_varint(&encoding_length);
    payload_bytes -= encoding_length;
    should_decompress = data_length > 0;
  }

  LOG(INFO) << "tpp: ok loading " << payload_bytes << " into packet payload";
  ByteBuffer payload(m_data_buffer.read_ubytes(payload_bytes));
  if (should_decompress) {
    BufferCompressor::decompress(payload);
  }

  LOG(INFO) << "tpp: " << m_data_buffer.get_data_length() << " bytes remaining in connection buffer";
  return payload;
}

void Connection::process_buffer() {
  LOG(INFO) << "process buffer called, popping as many packets as possible";

  std::vector<ByteBuffer> packets;
  while (auto maybe_packet = try_pop_packet()) {
    LOG(INFO) << "pb: pop packet success";
    packets.push_back(std::move(*maybe_packet));
  }

  LOG(INFO) << "pb: packet popping finished, read " << packets.size()
            << " packets from the buffer; posting for processing";
  for (auto packet : packets) {
    LOG(INFO) << "pb: posting to packet handler strand";
    boost::asio::post(incoming_packet_strand_, [packet = std::move(packet), self = shared_from_this()] mutable {
      LOG(INFO) << "strand: packet handler strand running";
      const auto packet_id = packet.read_varint();

      LOG(INFO) << "strand: read packet id " << packet_id;
      const auto packet_info = PacketRegistry::get_instance().get_serverbound(self->state_, packet_id);
      if (!packet_info || !packet_info->factory || !packet_info->handler) return;

      LOG(INFO) << "strand: running handler";
      packet_info->handler(*packet_info->factory(packet), *self);
    });
  }
}

ByteBuffer create_packet_standard_format(const int32_t packet_id, const ByteBuffer& payload_buffer) {
  ByteBuffer wrapper_buffer;
  wrapper_buffer.write_varint(
      static_cast<int32_t>(VarInt::encoding_length(packet_id) + payload_buffer.get_data_length()));
  wrapper_buffer.write_varint(packet_id);
  wrapper_buffer.write_ubytes(payload_buffer.get_bytes());
  return wrapper_buffer;
}

ByteBuffer create_packet_compression_format(const int32_t packet_id, const ByteBuffer& payload_buffer) {
  const bool should_compress =
      payload_buffer.get_data_length() >
      MinecraftServer::get_server().get_config_manager().get_server_config().get_compression_threshold();

  const auto data_length =
      should_compress ? static_cast<int32_t>(VarInt::encoding_length(packet_id) + payload_buffer.get_data_length()) : 0;

  ByteBuffer intermediate;
  intermediate.write_varint(packet_id);
  intermediate.write_ubytes(payload_buffer.get_bytes());
  if (should_compress) {
    intermediate = BufferCompressor::compress(intermediate, ZLIB);
  }

  ByteBuffer wrapper_buffer;
  wrapper_buffer.write_varint(
      static_cast<int32_t>(VarInt::encoding_length(data_length) + payload_buffer.get_data_length()));
  wrapper_buffer.write_varint(data_length);
  wrapper_buffer.write_ubytes(intermediate.get_bytes());
  return wrapper_buffer;
}

void Connection::send_packet(const int32_t packet_id, const ByteBuffer& payload_buffer) {
  ByteBuffer wrapper_buffer;
  if (is_compression_enabled()) {
    wrapper_buffer = create_packet_compression_format(packet_id, payload_buffer);
  } else {
    wrapper_buffer = create_packet_standard_format(packet_id, payload_buffer);
  }

  if (is_encryption_enabled()) {
    wrapper_buffer = get_buffer_crypter().encrypt(wrapper_buffer);
  }

  boost::asio::async_write(m_socket, boost::asio::buffer(wrapper_buffer.get_bytes()), boost::asio::transfer_all(),
                           [this](const boost::system::error_code& err, const std::size_t bytes_transferred) {
                             if (err) {
                               LOG(ERROR) << "Failed to write packet: " << err.message();
                             } else {
                               LOG(INFO) << "Wrote packet: " << bytes_transferred << " bytes written";
                             }
                           });
}

boost::asio::ip::tcp::socket* Connection::get_socket() { return &m_socket; }

bool Connection::set_context_value(const std::string& key, const boost::any& value) {
  return m_context_map.insert_or_assign(key, value).second;
}

std::optional<boost::any> Connection::get_context_value(const std::string& key) const {
  const auto search = m_context_map.find(key);
  return search == m_context_map.end() ? std::nullopt : search->second;
}

bool Connection::has_context_value(const std::string& key) const { return m_context_map.contains(key); }

void Connection::set_state(const ConnectionState state) { state_ = state; }

ConnectionState Connection::get_state() const { return state_; }

const std::shared_ptr<uuids::uuid>& Connection::get_unique_id() const { return m_unique_id; }

void Connection::set_unique_id(const std::shared_ptr<uuids::uuid>& unique_id) { m_unique_id = unique_id; }

int64_t Connection::get_keep_alive_payload() const {
  const auto keep_alive = get_context_value("keep_alive");
  if (!keep_alive || keep_alive->empty()) {
    return -1;
  }

  try {
    return boost::any_cast<int64_t>(*keep_alive);
  } catch (boost::bad_any_cast&) {
    return -1;
  }
}

bool Connection::is_compression_enabled() const {
  const auto compression = get_context_value("compression");
  if (!compression || compression->empty()) {
    return false;
  }

  try {
    return boost::any_cast<bool>(*compression);
  } catch (boost::bad_any_cast&) {
    return false;
  }
}

void Connection::enable_compression() { set_context_value("compression", true); }

bool Connection::is_encryption_enabled() const {
  const auto encryption = get_context_value("encryption");
  if (!encryption || encryption->empty()) {
    return false;
  }

  try {
    return boost::any_cast<bool>(*encryption);
  } catch (boost::bad_any_cast&) {
    return false;
  }
}

void Connection::enable_encryption(const std::vector<uint8_t>& shared_secret) {
  if (is_encryption_enabled() || m_buffer_crypter != nullptr) return;

  set_context_value("encryption", true);
  m_buffer_crypter = std::make_unique<BufferCrypter>(shared_secret);
}

BufferCrypter& Connection::get_buffer_crypter() const {
  if (m_buffer_crypter == nullptr) {
    throw std::runtime_error("Encryption is not enabled");
  }

  return *m_buffer_crypter;
}

void Connection::set_known_packs(std::vector<KnownPack> known_packs) { m_known_packs = std::move(known_packs); }

std::vector<KnownPack> Connection::get_known_packs() const { return m_known_packs; }

void Connection::send_disconnection(const std::string& reason) {
  switch (state_) {
    case ConnectionState::kLogin:
      this->send_packet(login::client::LoginDisconnectPacket(reason));
      break;
    case ConnectionState::kConfiguration:
      this->send_packet(configuration::client::DisconnectPacket(reason));
      break;
    case ConnectionState::kPlay:
      LOG(INFO) << "send_disconnection called in Play state; no Disconnection packet defined";
      break;
    case ConnectionState::kHandshaking:
    case ConnectionState::kStatus:
      // Nothing to do here. We'll just close the connection.
      break;
  }

  this->close();
}

void shutdown_socket(boost::asio::ip::tcp::socket& socket) {
  boost::system::error_code err;
  socket.shutdown(boost::asio::socket_base::shutdown_both, err);
  if (err && err != boost::asio::error::not_connected && err != boost::asio::error::bad_descriptor) {
    LOG(WARNING) << "Socket shutdown error: " << err.what();
  }
}

void Connection::close() {
  disconnect_callback_(shared_from_this());
  if (m_socket.is_open()) {
    LOG(INFO) << "Closing connection from " << m_socket.remote_endpoint().address();
    shutdown_socket(m_socket);
    m_socket.close();
  }
}

std::shared_ptr<OwnerTrackingSchedulerProxy> Connection::get_scheduler() const { return scheduler_; }
}  // namespace celerity::net