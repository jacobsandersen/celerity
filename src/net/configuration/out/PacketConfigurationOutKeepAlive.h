#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONOUTKEEPALIVE_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONOUTKEEPALIVE_H

#include "../../OutboundPacket.h"

namespace celerity::net::configuration {
class PacketConfigurationOutKeepAlive : public OutboundPacket {
 public:
  PacketConfigurationOutKeepAlive() : OutboundPacket(0x04) {}

  void write_data(ByteBuffer& buffer) override;
};
}  // namespace celerity::net::configuration

#endif
