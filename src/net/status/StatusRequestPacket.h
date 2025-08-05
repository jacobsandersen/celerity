//
// Created by Jacob Andersen on 8/5/25.
//

#ifndef STATUSREQUESTPACKET_H
#define STATUSREQUESTPACKET_H

#include "src/net/Packet.h"

namespace celerity::net::status {

struct StatusRequestPacket final : Packet {
  explicit StatusRequestPacket([[maybe_unused]] ByteBuffer& buffer) {}

  static void handle(const StatusRequestPacket& packet, Connection& connection);
};

}  // namespace celerity::net::status

#endif  // STATUSREQUESTPACKET_H
