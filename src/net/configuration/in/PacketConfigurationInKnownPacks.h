//
// Created by Jacob Andersen on 7/4/25.
//

#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINKNOWNPACKS_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINKNOWNPACKS_H

#include "../../Packet.h"

namespace celerity::net::configuration {
class PacketConfigurationInKnownPacks final : public Packet {
 public:
  void handle(const std::shared_ptr<Connection>& conn,
              const std::unique_ptr<ByteBuffer>& buffer) override;
};
}  // namespace celerity::net::configuration

#endif
