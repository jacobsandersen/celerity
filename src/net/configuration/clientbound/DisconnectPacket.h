#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONOUTDISCONNECT_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONOUTDISCONNECT_H

#include <utility>

#include "src/net/Packet.h"

namespace celerity::net::configuration::client {
struct DisconnectPacket final : Packet {
  std::string reason_;

  explicit DisconnectPacket(std::string reason) : reason_(std::move(reason)) {}

  [[nodiscard]] ByteBuffer encode() const;
};
}  // namespace celerity::net::configuration::client

#endif
