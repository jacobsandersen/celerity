//
// Created by Jacob Andersen on 7/30/25.
//

#include "ServerConfig.h"

constexpr uint16_t kDefaultServerPort = 25565;
constexpr uint16_t kDefaultCompressionThreshold = 256;

uint16_t ServerConfig::get_server_port() const { return toml::find_or<uint16_t>(toml_, "port", kDefaultServerPort); }

uint16_t ServerConfig::get_compression_threshold() const {
  return toml::find_or<uint16_t>(toml_, "compression_threshold", kDefaultCompressionThreshold);
}
uint16_t ServerConfig::get_max_players() const { return toml::find_or<uint16_t>(toml_, "max_players", 64); }

std::string ServerConfig::get_motd() const {
  return toml::find_or<std::string>(toml_, "motd", "A C++ Minecraft Server");
}

std::string ServerConfig::get_favicon() const { return toml::find_or<std::string>(toml_, "favicon", ""); }