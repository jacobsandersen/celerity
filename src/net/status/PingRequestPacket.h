//
// Created by Jacob Andersen on 8/5/25.
//

#ifndef PINGREQUESTPACKET_H
#define PINGREQUESTPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::status {
struct PingRequestPacket final : Packet {
  int64_t timestamp_;

  explicit PingRequestPacket(ByteBuffer& buffer) : timestamp_(buffer.read_be_long()) {}

  static void handle(const PingRequestPacket& packet, Connection& connection);
};
}  // namespace celerity::net::status

#endif //PINGREQUESTPACKET_H
