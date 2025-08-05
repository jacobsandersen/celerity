#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINPLUGINMESSAGE_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINPLUGINMESSAGE_H

#include "../../Packet.h"

namespace celerity::net::configuration {
class PacketConfigurationInPluginMessage : public Packet {
  void handle(const std::shared_ptr<Connection>&,
              const std::unique_ptr<ByteBuffer>&) override;
};
}  // namespace celerity::net::configuration

#endif
