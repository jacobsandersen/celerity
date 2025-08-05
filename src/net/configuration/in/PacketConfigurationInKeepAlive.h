#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINKEEPALIVE_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINKEEPALIVE_H

#include "../../Packet.h"

namespace celerity::net::configuration {
class PacketConfigurationInKeepAlive : public Packet {
 public:
  void handle(const std::shared_ptr<Connection>& conn, const std::unique_ptr<ByteBuffer>& buffer) override;
};
}  // namespace celerity::net::configuration

#endif
