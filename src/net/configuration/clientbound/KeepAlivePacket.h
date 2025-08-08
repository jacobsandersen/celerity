#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONOUTKEEPALIVE_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONOUTKEEPALIVE_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {
struct KeepAlivePacket final : Packet {
  int64_t payload_;

  explicit KeepAlivePacket(const int64_t payload) : payload_(payload) {}

  [[nodiscard]] ByteBuffer encode() const;
};
}  // namespace celerity::net::configuration::client

#endif
