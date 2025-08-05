//
// Created by Jacob Andersen on 8/5/25.
//

#ifndef STATUSPACKETREGISTRY_H
#define STATUSPACKETREGISTRY_H
#include "src/net/PacketRegistry.h"

namespace celerity::net::status {
struct StatusPacketRegistry {
  static void register_all(PacketRegistry& registry);
};

inline struct StatusRegistrations {
  StatusRegistrations() { PacketRegistry::add_init_function(StatusPacketRegistry::register_all); }
} _status_registration;
}  // namespace celerity::net::status

#endif  // STATUSPACKETREGISTRY_H
