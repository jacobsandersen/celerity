//
// Created by Jacob Andersen on 8/6/25.
//

#ifndef CELERITY_NET_CONFIGURATION_CLIENTBOUND_FINISHCONFIGURATIONPACKET_H
#define CELERITY_NET_CONFIGURATION_CLIENTBOUND_FINISHCONFIGURATIONPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {
struct FinishConfigurationPacket final : Packet {
  [[nodiscard]] ByteBuffer encode() const;
};
}  // namespace celerity::net::configuration::client

#endif  // CELERITY_NET_CONFIGURATION_CLIENTBOUND_FINISHCONFIGURATIONPACKET_H
