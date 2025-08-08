#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINKEEPALIVE_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINKEEPALIVE_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::server {
struct KeepAlivePacket final : Packet {
  int64_t payload_;

  explicit KeepAlivePacket(ByteBuffer buffer) : payload_(buffer.read_be_long()) {}

  static void handle(const KeepAlivePacket& packet, Connection& connection);
};
}  // namespace celerity::net::configuration::server

#endif
