#ifndef CELERITY_NET_CONNECTION_H
#define CELERITY_NET_CONNECTION_H

#include <cryptopp/modes.h>
#include <uuid.h>

#include <boost/any.hpp>
#include <boost/asio.hpp>

#include "../BasicTimer.h"
#include "../BufferCrypter.h"
#include "../ByteBuffer.h"
#include "../KnownPack.h"

namespace celerity::net {
enum class ConnectionState { Handshaking, Status, Login, Configuration, Play };

struct DecodedPacket {
  int32_t packet_id;
  ByteBuffer payload;
};

class Connection : std::enable_shared_from_this<Connection> {
 public:
  explicit Connection(boost::asio::ip::tcp::socket socket,
                      std::function<void(Connection*)> disconnect_callback)
      : m_socket(std::move(socket)),
        disconnect_callback_(std::move(disconnect_callback)),
        incoming_packet_strand_(m_socket.get_executor()) {}

  void start();

  [[nodiscard]] boost::asio::ip::tcp::socket* get_socket();

  [[nodiscard]] ConnectionState get_state() const;

  void set_state(ConnectionState state);

  bool set_context_value(const std::string& key, boost::any value);

  [[nodiscard]] std::optional<boost::any> get_context_value(
      const std::string& key) const;

  [[nodiscard]] const std::shared_ptr<uuids::uuid>& get_unique_id() const;

  void set_unique_id(const std::shared_ptr<uuids::uuid>& unique_id);

  void start_new_timer(std::chrono::seconds timeout,
                       const std::function<void()>& callback);

  [[nodiscard]] int64_t get_keep_alive_payload() const;

  [[nodiscard]] bool is_compression_enabled() const;

  void enable_compression();

  [[nodiscard]] bool is_encryption_enabled() const;

  void enable_encryption(const std::vector<uint8_t>& shared_secret);

  BufferCrypter& get_buffer_crypter() const;

  void set_known_packs(std::vector<KnownPack> known_packs);

  std::vector<KnownPack> get_known_packs() const;

  void unclean_close();

 private:
  bool running_{};
  boost::asio::ip::tcp::socket m_socket;
  ByteBuffer m_data_buffer{};
  ConnectionState m_state = ConnectionState::Handshaking;
  std::map<std::string, boost::any> m_context_map;
  std::unique_ptr<BasicTimer> m_timer{};
  std::unique_ptr<BufferCrypter> m_buffer_crypter{};
  std::shared_ptr<uuids::uuid> m_unique_id{};
  std::vector<KnownPack> m_known_packs{};
  std::function<void(Connection*)> disconnect_callback_;
  boost::asio::strand<boost::asio::any_io_executor> incoming_packet_strand_;

  void do_read();
  std::optional<DecodedPacket> try_pop_packet();
  void process_buffer();
  void process_packet(DecodedPacket&& packet);
};
}  // namespace celerity::net

#endif
