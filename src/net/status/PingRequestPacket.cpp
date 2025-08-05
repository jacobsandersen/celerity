//
// Created by Jacob Andersen on 8/5/25.
//

#include "PingRequestPacket.h"

#include "PongResponsePacket.h"
#include "src/net/Connection.h"

namespace celerity::net::status {
void PingRequestPacket::handle(const PingRequestPacket& packet, Connection& connection) {
  connection.send_packet(PongResponsePacket{packet.timestamp_});
}
}  // namespace celerity::net::status