#include "NetworkManager.h"

#include <absl/log/log.h>

#include <boost/endian/detail/endian_reverse.hpp>

#include "../BufferCompressor.h"
#include "../MinecraftServer.h"

namespace celerity::net {
using namespace boost::asio::ip;

NetworkManager::NetworkManager(boost::asio::io_context& io_context, std::shared_ptr<Scheduler>& scheduler,
                               const ServerConfig& config)
    : io_context_(io_context),
      scheduler_(scheduler),
      acceptor_(tcp::acceptor(io_context_, tcp::endpoint(tcp::v4(), config.get_server_port()))) {
  LOG(INFO) << "Accepting connections on " << acceptor_.local_endpoint();
}

void NetworkManager::start() {
  acceptor_.async_accept([this](const boost::system::error_code& err, tcp::socket sock) {
    if (err) {
      if (err == boost::asio::error::operation_aborted) {
        return;
      }

      LOG(WARNING) << "Failed to accept a new connection: " << err.message();
    } else {
      LOG(INFO) << "Connection established from " << sock.remote_endpoint().address();

      const auto conn =
          std::make_shared<Connection>(std::move(sock), scheduler_, [this](const std::shared_ptr<Connection>& c) {
            if (c == nullptr) return;
            MinecraftServer::get_server().remove_player(c->get_unique_id());
            if (const auto it = std::ranges::find(connections_, c); it != connections_.end()) {
              connections_.erase(it);
            }
          });

      connections_.push_back(conn);

      conn->start();
    }
    start();
  });
}

void NetworkManager::shutdown() {
  const auto conns = std::move(connections_);
  connections_.clear();

  for (auto& conn : conns) {
    conn->send_disconnection("Server is shutting down");
  }

  acceptor_.cancel();
  acceptor_.close();
}
}  // namespace celerity::net