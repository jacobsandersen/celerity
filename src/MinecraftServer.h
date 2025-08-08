#ifndef CELERITY_MINECRAFTSERVER_H
#define CELERITY_MINECRAFTSERVER_H

#include <boost/asio.hpp>

#include "ConfigManager.h"
#include "RSAKeypair.h"
#include "net/Connection.h"
#include "net/NetworkManager.h"
#include "player/Player.h"

using boost::asio::ip::tcp;

namespace celerity {
class MinecraftServer {
 public:
  static MinecraftServer* get_server();

  void start();

  [[nodiscard]] const net::NetworkManager& get_network_manager() const;

  [[nodiscard]] const ConfigManager& get_config_manager() const;

  [[nodiscard]] const RSAKeypair& get_rsa_keypair() const;

  const std::string& get_version_name() const;

  [[nodiscard]] uint32_t get_protocol_version() const;

  std::vector<std::shared_ptr<player::Player>> get_players();

  std::shared_ptr<player::Player> get_player(const std::string& username);

  std::shared_ptr<player::Player> get_player(const std::shared_ptr<uuids::uuid>& unique_id);

  void remove_player(const std::shared_ptr<uuids::uuid>& unique_id);

  void add_player(const std::shared_ptr<player::Player>&);

  [[nodiscard]] std::filesystem::path get_server_root() const;

  [[nodiscard]] const std::vector<KnownPack>& get_known_packs() const;

 private:
  MinecraftServer()
      : server_root_(std::filesystem::current_path()),
        config_manager_(server_root_),
        network_manager_(config_manager_.get_server_config()),
        version_name_("1.21.7"),
        protocol_version_(772) {
    known_packs_.emplace_back("minecraft", "core", "1.21.5");
  }

  std::filesystem::path server_root_;
  ConfigManager config_manager_;
  net::NetworkManager network_manager_;
  RSAKeypair rsa_keypair_;
  std::string version_name_;
  uint32_t protocol_version_;
  std::vector<KnownPack> known_packs_;
  std::vector<std::shared_ptr<player::Player>> players_{};
};
}  // namespace celerity
#endif
