//
// Created by Jacob Andersen on 8/5/25.
//

#include "StatusPacketRegistry.h"

namespace celerity::net::status {
template <typename T>
  requires ServerboundPacket<T>
void register_serverbound(PacketRegistry& registry) {
  registry.register_serverbound<T>(ConnectionState::kStatus);
}

template <typename T>
  requires ClientboundPacket<T>
void register_clientbound(PacketRegistry& registry) {
  registry.register_clientbound<T>(ConnectionState::kStatus);
}

void StatusPacketRegistry::register_all(PacketRegistry& registry) {}
}  // namespace celerity::net::status