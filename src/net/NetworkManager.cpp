#include "NetworkManager.h"

#include <absl/log/log.h>

#include "../BufferCompressor.h"
#include "../MinecraftServer.h"

namespace celerity::net {

void NetworkManager::start() {
  const uint32_t thread_count =
      std::max(2u, std::thread::hardware_concurrency());

  for (int i = 0; i < thread_count; ++i) {
    network_threads_.emplace_back([this] {
      try {
        context_.run();
      } catch (const std::exception& err) {
        LOG(FATAL) << "Network thread crashed: " << err.what() << std::endl;
        shutdown();
      }
    });
  }

  accept_connection();
  LOG(INFO) << "Celerity is listening for connections on "
            << acceptor_.local_endpoint();
}

void NetworkManager::shutdown() {
  LOG(INFO) << "NetworkManager shutting down...";

  context_.stop();
  for (auto& network_thread : network_threads_) {
    if (network_thread.joinable()) {
      network_thread.join();
    }
  }

  LOG(INFO) << "NetworkManager stopped.";
}

void NetworkManager::accept_connection() {
  acceptor_.async_accept([this](const boost::system::error_code& err,
                                tcp::socket sock) {
    if (err) {
      LOG(WARNING) << "Failed to accept a new connection: " << err.message();
    } else {
      LOG(INFO) << "Connection established from "
                << sock.remote_endpoint().address().to_string();

      const auto conn = std::make_shared<Connection>(
          std::move(sock), [this](const Connection* c) {
            if (c == nullptr) return;
            MinecraftServer::get_server()->remove_player(c->get_unique_id());
          });

      conn->start();
    }
    accept_connection();
  });
}
}  // namespace celerity::net