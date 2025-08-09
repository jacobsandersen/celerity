//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef CELERITY_NET_CONFIGURATION_SERVERBOUND_FINISHCONFIGURATIONPACKET_H
#define CELERITY_NET_CONFIGURATION_SERVERBOUND_FINISHCONFIGURATIONPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::server {

struct FinishConfigurationPacket final : Packet {
  explicit FinishConfigurationPacket([[maybe_unused]] ByteBuffer buffer) {}

  static void handle(const FinishConfigurationPacket& packet, Connection& connection);
};

}  // namespace celerity::net::configuration::server

#endif //CELERITY_NET_CONFIGURATION_SERVERBOUND_FINISHCONFIGURATIONPACKET_H
