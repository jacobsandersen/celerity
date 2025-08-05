#include "MinecraftServer.h"

#include "VarInt.h"

namespace celerity {
MinecraftServer* instance = nullptr;

MinecraftServer* MinecraftServer::get_server() {
  if (!instance) {
    instance = new MinecraftServer();
  }

  return instance;
}

void MinecraftServer::start() { network_manager_.start(); }

std::vector<std::shared_ptr<player::Player>> MinecraftServer::get_players() {
  return players_;
}

std::shared_ptr<player::Player> MinecraftServer::get_player(
    const std::string& username) {
  for (std::shared_ptr<player::Player> player : get_players()) {
    if (player->get_username() == username) {
      return player;
    }
  }

  return nullptr;
}

std::shared_ptr<player::Player> MinecraftServer::get_player(
    const std::shared_ptr<uuids::uuid>& unique_id) {
  for (std::shared_ptr<player::Player> player : get_players()) {
    if (player->get_unique_id() == unique_id) {
      return player;
    }
  }

  return nullptr;
}

void MinecraftServer::remove_player(
    const std::shared_ptr<uuids::uuid>& unique_id) {
  if (unique_id == nullptr) return;
  players_.erase(
      std::remove_if(players_.begin(), players_.end(),
                     [unique_id](std::shared_ptr<player::Player> player) {
                       return player->get_unique_id() == unique_id;
                     }),
      players_.end());
}

const net::NetworkManager& MinecraftServer::get_network_manager() const {
  return network_manager_;
}

const RSAKeypair& MinecraftServer::get_rsa_keypair() const {
  return rsa_keypair_;
}

void MinecraftServer::add_player(
    const std::shared_ptr<player::Player>& player) {
  players_.push_back(player);
}
std::filesystem::path MinecraftServer::get_server_root() const {
  return server_root_;
}

const ConfigManager& MinecraftServer::get_config_manager() const {
  return config_manager_;
}

uint32_t MinecraftServer::get_protocol_version() const {
  return protocol_version_;
}

const std::string& MinecraftServer::get_version_name() const {
  return version_name_;
}

const std::vector<KnownPack>& MinecraftServer::get_known_packs() const {
  return known_packs_;
}
}  // namespace celerity