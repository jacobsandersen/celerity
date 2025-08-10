//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef PLAYPACKETREGISTRY_H
#define PLAYPACKETREGISTRY_H
#include "src/net/PacketRegistry.h"

namespace celerity::net::play {
struct PlayPacketRegistry {
  static void register_all(PacketRegistry& registry);
};

inline struct PlayRegistrations {
  PlayRegistrations() { PacketRegistry::add_init_function(PlayPacketRegistry::register_all); }
} _play_registration;
}  // namespace celerity::net::play

#endif  // PLAYPACKETREGISTRY_H
