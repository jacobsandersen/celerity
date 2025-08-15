#include "MinecraftServer.h"

#include <iostream>

#include "VarInt.h"
#include "registry/RegistryLoader.h"

namespace celerity {
MinecraftServer* instance = nullptr;

MinecraftServer& MinecraftServer::get_server() {
  static MinecraftServer server;
  return server;
}

void MinecraftServer::start() {
  LOG(INFO) << "Initializing packet registry...";
  net::PacketRegistry::get_instance();

  LOG(INFO) << "Loading data registries...";
  registries_ = registry::RegistryLoader::load_registries(server_root_);

  LOG(INFO) << "Spawning IO threads...";
  const auto work_guard = boost::asio::make_work_guard(io_context_);
  const uint32_t thread_count = std::max(2u, std::thread::hardware_concurrency());
  std::vector<std::thread> io_threads(thread_count);
  for (int i = 0; i < thread_count; ++i) {
    io_threads.emplace_back([this] {
      try {
        io_context_.run();
      } catch (const std::exception& err) {
        LOG(FATAL) << "Network thread crashed: " << err.what() << std::endl;
      }
    });
  }

  LOG(INFO) << "Starting network manager...";
  network_manager_.start();

  LOG(INFO) << "Initializing console...";
  std::thread repl_thread([this] { repl_loop(); });
  repl_thread.detach();

  LOG(INFO) << "Celerity is running. Type 'help' for a list of commands.";
  game_loop();

  LOG(INFO) << "Shutting down network manager...";
  network_manager_.shutdown();

  LOG(INFO) << "Shutting down IO threads...";
  io_context_.stop();
  for (auto& io_thread : io_threads) {
    if (io_thread.joinable()) {
      const auto id = io_thread.get_id();
      io_thread.join();
      LOG(INFO) << "IO thread " << id << " stopped";
    }
  }
}

void MinecraftServer::stop() { running_ = false; }

std::vector<std::shared_ptr<player::Player>> MinecraftServer::get_players() { return players_; }

std::shared_ptr<player::Player> MinecraftServer::get_player(const std::string& username) {
  for (std::shared_ptr player : get_players()) {
    if (player->get_username() == username) {
      return player;
    }
  }

  return nullptr;
}

std::shared_ptr<player::Player> MinecraftServer::get_player(const std::shared_ptr<uuids::uuid>& unique_id) {
  for (std::shared_ptr player : get_players()) {
    if (player->get_unique_id() == unique_id) {
      return player;
    }
  }

  return nullptr;
}

void MinecraftServer::remove_player(const std::shared_ptr<uuids::uuid>& unique_id) {
  if (unique_id == nullptr) return;
  players_.erase(std::ranges::remove_if(players_,
                                        [unique_id](const std::shared_ptr<player::Player>& player) {
                                          return player->get_unique_id() == unique_id;
                                        })
                     .begin(),
                 players_.end());
}

const net::NetworkManager& MinecraftServer::get_network_manager() const { return network_manager_; }

const RSAKeypair& MinecraftServer::get_rsa_keypair() const { return rsa_keypair_; }

void MinecraftServer::add_player(const std::shared_ptr<player::Player>& player) { players_.push_back(player); }
std::filesystem::path MinecraftServer::get_server_root() const { return server_root_; }

const ConfigManager& MinecraftServer::get_config_manager() const { return config_manager_; }

uint32_t MinecraftServer::get_protocol_version() const { return protocol_version_; }

const std::string& MinecraftServer::get_version_name() const { return version_name_; }

const std::vector<KnownPack>& MinecraftServer::get_known_packs() const { return known_packs_; }

void MinecraftServer::repl_loop() {
  std::string line;
  while (running_ && std::getline(std::cin, line)) {
    if (line == "stop") {
      running_ = false;
      break;
    }

    LOG(INFO) << "Unknown command.";
  }
}

void MinecraftServer::game_loop() {
  auto next_tick = std::chrono::steady_clock::now();
  while (running_) {
    tick();
    next_tick += std::chrono::milliseconds(50);
    std::this_thread::sleep_until(next_tick);
  }
}

void MinecraftServer::tick() {}
}  // namespace celerity