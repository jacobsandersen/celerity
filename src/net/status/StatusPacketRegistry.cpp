//
// Created by Jacob Andersen on 8/5/25.
//

#include "StatusPacketRegistry.h"

#include "clientbound/PongResponsePacket.h"
#include "clientbound/StatusResponsePacket.h"
#include "serverbound/PingRequestPacket.h"
#include "serverbound/StatusRequestPacket.h"

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

void StatusPacketRegistry::register_all(PacketRegistry& registry) {
  register_clientbound<client::StatusResponsePacket>(registry);
  register_clientbound<client::PongResponsePacket>(registry);

  register_serverbound<server::StatusRequestPacket>(registry);
  register_serverbound<server::PingRequestPacket>(registry);
}
}  // namespace celerity::net::status