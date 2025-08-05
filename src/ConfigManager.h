#ifndef CELERITY_CONFIGMANAGER_H
#define CELERITY_CONFIGMANAGER_H

#include <toml.hpp>

#include "ServerConfig.h"

namespace celerity {
class ConfigManager {
 public:
  explicit ConfigManager(const std::filesystem::path& server_root) : server_config_(server_root) {}

  [[nodiscard]] const ServerConfig& get_server_config() const;

 private:
  ServerConfig server_config_;
};
}  // namespace celerity

#endif
