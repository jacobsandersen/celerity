#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONOUTDISCONNECT_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONOUTDISCONNECT_H

#include <utility>

#include "../../OutboundPacket.h"

namespace celerity::net::configuration {
class PacketConfigurationOutDisconnect : public OutboundPacket {
 public:
  explicit PacketConfigurationOutDisconnect(std::string reason)
      : OutboundPacket(0x02), m_reason(std::move(reason)) {}

  void write_data(ByteBuffer&) override;

 private:
  std::string m_reason;
};
}  // namespace celerity::net::configuration

#endif
