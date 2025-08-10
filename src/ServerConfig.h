//
// Created by Jacob Andersen on 7/30/25.
//

#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H
#include <absl/log/log.h>

#include <toml.hpp>

class ServerConfig {
  toml::value toml_;

 public:
  explicit ServerConfig(const std::filesystem::path& server_root)
      : toml_(toml::parse(server_root / "config/server.toml")) {}

  [[nodiscard]] uint16_t get_server_port() const;
  [[nodiscard]] uint16_t get_compression_threshold() const;
  [[nodiscard]] uint16_t get_max_players() const;
  [[nodiscard]] std::string get_motd() const;
  [[nodiscard]] std::string get_favicon() const;
};

#endif  // SERVERCONFIG_H
