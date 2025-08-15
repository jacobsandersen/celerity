#ifndef CELERITY_MINECRAFTSERVER_H
#define CELERITY_MINECRAFTSERVER_H

#include <boost/asio.hpp>

#include "ConfigManager.h"
#include "RSAKeypair.h"
#include "Scheduler.h"
#include "net/NetworkManager.h"
#include "player/Player.h"
#include "registry/Registry.h"

namespace celerity {
class MinecraftServer {
 public:
  [[nodiscard]] static MinecraftServer& get_server();

  void start();

  void stop();

  [[nodiscard]] const net::NetworkManager& get_network_manager() const;

  [[nodiscard]] const ConfigManager& get_config_manager() const;

  [[nodiscard]] const RSAKeypair& get_rsa_keypair() const;

  [[nodiscard]] const std::string& get_version_name() const;

  [[nodiscard]] uint32_t get_protocol_version() const;

  std::vector<std::shared_ptr<player::Player>> get_players();

  std::shared_ptr<player::Player> get_player(const std::string& username);

  std::shared_ptr<player::Player> get_player(const std::shared_ptr<uuids::uuid>& unique_id);

  void remove_player(const std::shared_ptr<uuids::uuid>& unique_id);

  void add_player(const std::shared_ptr<player::Player>&);

  [[nodiscard]] std::filesystem::path get_server_root() const;

  [[nodiscard]] const std::vector<KnownPack>& get_known_packs() const;

  [[nodiscard]] std::shared_ptr<Scheduler> get_scheduler() { return scheduler_; }

  [[nodiscard]] const std::vector<registry::Registry>& get_registries();

 private:
  MinecraftServer()
      : server_root_(std::filesystem::current_path()),
        config_manager_(server_root_),
        network_manager_(io_context_, scheduler_, config_manager_.get_server_config()),
        version_name_("1.21.8"),
        protocol_version_(772),
        scheduler_(std::make_shared<Scheduler>(io_context_)) {
    known_packs_.emplace_back("minecraft", "core", "1.21.8");
  }

  void repl_loop();
  void game_loop();
  void tick();

  std::atomic_bool running_{true};
  boost::asio::io_context io_context_;
  std::filesystem::path server_root_;
  ConfigManager config_manager_;
  net::NetworkManager network_manager_;
  RSAKeypair rsa_keypair_;
  std::string version_name_;
  uint32_t protocol_version_;
  std::vector<KnownPack> known_packs_;
  std::vector<std::shared_ptr<player::Player>> players_{};
  std::shared_ptr<Scheduler> scheduler_;
  std::vector<registry::Registry> registries_;
};
}  // namespace celerity
#endif
