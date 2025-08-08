//
// Created by Jacob Andersen on 8/8/25.
//

#ifndef PINGPACKET_H
#define PINGPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {
struct PingPacket final : Packet {
  int32_t id_;

  explicit PingPacket(const int32_t id) : id_(id) {}

  [[nodiscard]] ByteBuffer encode() const;
};
}  // namespace celerity::net::configuration::client

#endif  // PINGPACKET_H
