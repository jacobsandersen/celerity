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

void Connection::start() {
  if (running_) return;
  running_ = true;
  do_read();
}
void Connection::do_read() {
  std::vector<uint8_t> read_buffer;
  read_buffer.resize(kReadBufferSize);

  m_socket.async_read_some(boost::asio::buffer(read_buffer),
                           [this, read_buffer = std::move(read_buffer)](const boost::system::error_code& err,
                                                                        const std::size_t bytes_read) mutable {
                             if (err) {
                               LOG(ERROR) << "Failed to read socket: " + err.message();
                               this->close();
                               return;
                             }

                             read_buffer.resize(bytes_read);

                             ByteBuffer tmp(std::move(read_buffer));
                             if (this->is_encryption_enabled()) {
                               this->get_buffer_crypter().decrypt(tmp);
                             }

                             m_data_buffer.append(tmp);

                             process_buffer();
                             do_read();
                           });
}

std::optional<ByteBuffer> Connection::try_pop_packet() {
  const auto maybe_packet_length = m_data_buffer.peek_varint();
  if (!maybe_packet_length) {
    return std::nullopt;
  }

  const auto [packet_length, packet_length_num_bytes] = *maybe_packet_length;
  if (m_data_buffer.get_data_length() - packet_length_num_bytes < packet_length) {
    return std::nullopt;
  }

  m_data_buffer.truncate_front(packet_length_num_bytes);

  size_t payload_bytes = packet_length;
  bool should_decompress = false;

  if (is_compression_enabled()) {
    uint8_t encoding_length;
    const auto data_length = m_data_buffer.read_varint(&encoding_length);
    payload_bytes -= encoding_length;
    should_decompress = data_length > 0;
  }

  ByteBuffer payload(m_data_buffer.read_ubytes(payload_bytes));
  if (should_decompress) {
    BufferCompressor::decompress(payload);
  }

  return payload;
}

void Connection::process_buffer() {
  while (auto maybe_packet = try_pop_packet()) {
    ByteBuffer packet = std::move(*maybe_packet);
    boost::asio::post(incoming_packet_strand_, [packet = std::move(packet), self = shared_from_this()] mutable {
      const auto packet_id = packet.read_varint();

      const auto packet_info = self->registry_.get_serverbound(self->state_, packet_id);
      if (!packet_info || !packet_info->factory || !packet_info->handler) return;

      packet_info->handler(*packet_info->factory(packet), *self);
    });
  }
}

void Connection::send_packet(ByteBuffer payload_buffer) {
  bool should_compress = false;
  if (is_compression_enabled()) {
    const auto threshold =
        MinecraftServer::get_server()->get_config_manager().get_server_config().get_compression_threshold();

    should_compress = payload_buffer.get_data_length() > threshold;
  }

  const int32_t data_length = should_compress ? static_cast<int32_t>(payload_buffer.get_data_length()) : 0;
  if (should_compress) {
    payload_buffer = BufferCompressor::compress(payload_buffer, ZLIB);
  }

  ByteBuffer wrapperBuffer;
  wrapperBuffer.write_varint(
      static_cast<int32_t>(VarInt::encoding_length(data_length) + payload_buffer.get_data_length()));
  wrapperBuffer.write_varint(data_length);
  wrapperBuffer.write_ubytes(payload_buffer.get_bytes());

  if (is_encryption_enabled()) {
    wrapperBuffer = get_buffer_crypter().encrypt(wrapperBuffer);
  }

  boost::asio::async_write(m_socket, boost::asio::buffer(wrapperBuffer.get_bytes()), boost::asio::transfer_all(),
                           [this](const boost::system::error_code& err, const std::size_t bytes_transferred) {
                             if (err) {
                               LOG(ERROR) << "Failed to write packet: " << err.message();
                             } else {
                               LOG(INFO) << "Wrote packet: " << bytes_transferred << " bytes written";
                             }
                           });
}

tcp::socket* Connection::get_socket() { return &m_socket; }

bool Connection::set_context_value(const std::string& key, const boost::any& value) {
  return m_context_map.insert_or_assign(key, value).second;
}

std::optional<boost::any> Connection::get_context_value(const std::string& key) const {
  const auto search = m_context_map.find(key);
  return search == m_context_map.end() ? std::nullopt : search->second;
}
bool Connection::has_context_value(const std::string& key) const { return get_context_value(key) != std::nullopt; }

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

void Connection::close() {
  disconnect_callback_(this);
  m_socket.shutdown(boost::asio::socket_base::shutdown_both);
  m_socket.close();
}
}  // namespace celerity::net