//
// Created by Jacob Andersen on 8/9/25.
//

#include "PlayPacketRegistry.h"

#include <absl/log/log.h>

namespace celerity::net::play {
template <typename T>
  requires ServerboundPacket<T>
void register_serverbound(PacketRegistry& registry) {
  registry.register_serverbound<T>(ConnectionState::kPlay);
}

template <typename T>
  requires ClientboundPacket<T>
void register_clientbound(PacketRegistry& registry) {
  registry.register_clientbound<T>(ConnectionState::kPlay);
}

void PlayPacketRegistry::register_all(PacketRegistry& registry) { LOG(INFO) << "Registering play packets"; }
}  // namespace celerity::net::play