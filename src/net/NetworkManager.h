#ifndef CELERITY_NET_NETWORKMANAGER_H
#define CELERITY_NET_NETWORKMANAGER_H

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "Connection.h"
#include "src/ServerConfig.h"

using std::unordered_map, std::unique_ptr, std::make_unique;

namespace celerity::net {
class NetworkManager {
  boost::asio::io_context context_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::vector<std::thread> network_threads_;

 public:
  explicit NetworkManager(const ServerConfig& config)
      : acceptor_(boost::asio::ip::tcp::acceptor(
            context_,
            boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                           config.get_server_port()))) {}

  void start();
  void shutdown();

 private:
  void accept_connection();
};
}  // namespace celerity::net

#endif
