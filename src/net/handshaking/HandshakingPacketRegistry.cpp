//
// Created by Jacob Andersen on 8/5/25.
//

#include "HandshakingPacketRegistry.h"

#include <absl/log/log.h>

#include "serverbound/IntentionPacket.h"

namespace celerity::net::handshaking {
template <typename T>
  requires ServerboundPacket<T>
void register_serverbound(PacketRegistry& registry) {
  registry.register_serverbound<T>(ConnectionState::kHandshaking);
}

void HandshakingPacketRegistry::register_all(PacketRegistry& registry) {
  LOG(INFO) << "Registering handshaking packets";
  register_serverbound<server::IntentionPacket>(registry);
}
}  // namespace celerity::net::handshaking