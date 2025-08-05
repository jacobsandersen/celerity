#include "ConfigManager.h"

namespace celerity {
const ServerConfig& ConfigManager::get_server_config() const { return server_config_; }
}  // namespace celerity
