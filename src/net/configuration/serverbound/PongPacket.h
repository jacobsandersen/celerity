//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef PONGPACKET_H
#define PONGPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::server {

struct PongPacket final : Packet {
  int32_t id_;

  explicit PongPacket(ByteBuffer buffer) : id_(buffer.read_be_int()) {}

  static void handle(const PongPacket& packet, Connection& connection);
};

}  // namespace celerity::net::configuration::server

#endif  // PONGPACKET_H
