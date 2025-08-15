//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef REGISTRYDATAPACKET_H
#define REGISTRYDATAPACKET_H

#include "src/net/Packet.h"
#include "src/registry/Registry.h"

namespace celerity::net::configuration::client {

struct RegistryDataPacket final : Packet {
  const registry::Registry& registry_;

  explicit RegistryDataPacket(const registry::Registry& registry) : registry_(registry) {}

  ByteBuffer encode() const;
};

}  // namespace celerity::net::configuration::client

#endif  // REGISTRYDATAPACKET_H
