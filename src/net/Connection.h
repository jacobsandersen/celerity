#ifndef CELERITY_NET_CONNECTION_H
#define CELERITY_NET_CONNECTION_H

#include <absl/log/log.h>
#include <cryptopp/modes.h>
#include <uuid.h>

#include <boost/any.hpp>
#include <boost/asio.hpp>

#include "../BufferCrypter.h"
#include "../ByteBuffer.h"
#include "../KnownPack.h"
#include "ConnectionState.h"
#include "PacketRegistry.h"
#include "src/Scheduler.h"

namespace celerity::net {
class Connection : std::enable_shared_from_this<Connection> {
 public:
  explicit Connection(boost::asio::ip::tcp::socket socket, std::function<void(Connection*)> disconnect_callback)
      : m_socket(std::move(socket)),
        scheduler_(m_socket.get_executor()),
        disconnect_callback_(std::move(disconnect_callback)),
        incoming_packet_strand_(m_socket.get_executor()) {}

  ~Connection() { scheduler_.cancel_all(); }

  void start();

  [[nodiscard]] boost::asio::ip::tcp::socket* get_socket();

  bool set_context_value(const std::string& key, const boost::any& value);

  [[nodiscard]] std::optional<boost::any> get_context_value(const std::string& key) const;

  [[nodiscard]] bool has_context_value(const std::string& key) const;

  [[nodiscard]] Scheduler& get_scheduler() { return scheduler_; }

  void set_state(ConnectionState state);

  [[nodiscard]] ConnectionState get_state() const;

  [[nodiscard]] const std::shared_ptr<uuids::uuid>& get_unique_id() const;

  void set_unique_id(const std::shared_ptr<uuids::uuid>& unique_id);

  [[nodiscard]] int64_t get_keep_alive_payload() const;

  [[nodiscard]] bool is_compression_enabled() const;

  void enable_compression();

  [[nodiscard]] bool is_encryption_enabled() const;

  void enable_encryption(const std::vector<uint8_t>& shared_secret);

  BufferCrypter& get_buffer_crypter() const;

  void set_known_packs(std::vector<KnownPack> known_packs);

  std::vector<KnownPack> get_known_packs() const;

  void send_disconnection(const std::string& reason);

  void close();

  template <typename T>
    requires ClientboundPacket<T>
  void send_packet(T packet) {
    const auto info = registry_.get_clientbound<T>(state_);
    if (!info || !info->encoder) {
      LOG(ERROR) << "Attempted to send unknown packet " << info->packet_id << " for state " << state_;
      return;
    }

    send_packet(info->encoder(packet));
  }

 private:
  bool running_{};
  boost::asio::ip::tcp::socket m_socket;
  ByteBuffer m_data_buffer{};
  ConnectionState state_ = ConnectionState::kHandshaking;
  std::map<std::string, boost::any> m_context_map;
  Scheduler scheduler_;
  std::unique_ptr<BufferCrypter> m_buffer_crypter{};
  std::shared_ptr<uuids::uuid> m_unique_id{};
  std::vector<KnownPack> m_known_packs{};
  std::function<void(Connection*)> disconnect_callback_;
  boost::asio::strand<boost::asio::any_io_executor> incoming_packet_strand_;
  PacketRegistry registry_{};

  void do_read();
  std::optional<ByteBuffer> try_pop_packet();
  void process_buffer();
  void send_packet(ByteBuffer payload_buffer);
};
}  // namespace celerity::net

#endif
