//
// Created by Jacob Andersen on 8/5/25.
//

#ifndef HANDSHAKINGPACKETREGISTRY_H
#define HANDSHAKINGPACKETREGISTRY_H

#include "src/net/PacketRegistry.h"

namespace celerity::net::handshaking {
struct HandshakingPacketRegistry {
  static void register_all(PacketRegistry& registry);
};

inline struct HandshakingRegistration {
  HandshakingRegistration() {
    PacketRegistry::add_init_function(HandshakingPacketRegistry::register_all);
  }
} _handshaking_registration;
}  // namespace celerity::net

#endif //HANDSHAKINGPACKETREGISTRY_H
