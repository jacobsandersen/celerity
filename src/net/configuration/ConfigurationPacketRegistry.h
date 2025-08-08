//
// Created by Jacob Andersen on 8/8/25.
//

#ifndef CONFIGURATIONPACKETREGISTRY_H
#define CONFIGURATIONPACKETREGISTRY_H
#include "src/net/PacketRegistry.h"

namespace celerity::net::configuration {
struct ConfigurationPacketRegistry {
  static void register_all(PacketRegistry& registry);
};

inline struct ConfigurationRegistrations {
  ConfigurationRegistrations() { PacketRegistry::add_init_function(ConfigurationPacketRegistry::register_all); }
} _configuration_registration;
}  // namespace celerity::net::configuration

#endif  // CONFIGURATIONPACKETREGISTRY_H
