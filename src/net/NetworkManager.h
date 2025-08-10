#ifndef CELERITY_NET_NETWORKMANAGER_H
#define CELERITY_NET_NETWORKMANAGER_H

#include <boost/asio/ip/tcp.hpp>

#include "Connection.h"
#include "src/ServerConfig.h"

using std::unordered_map, std::unique_ptr, std::make_unique;

namespace celerity::net {
class NetworkManager {
  boost::asio::io_context& io_context_;
  std::shared_ptr<Scheduler>& scheduler_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::list<std::shared_ptr<Connection>> connections_;

 public:
  explicit NetworkManager(boost::asio::io_context& io_context, std::shared_ptr<Scheduler>& scheduler,
                          const ServerConfig& config);

  void start();
  void shutdown();
};
}  // namespace celerity::net

#endif
