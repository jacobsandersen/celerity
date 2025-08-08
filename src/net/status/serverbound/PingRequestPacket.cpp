//
// Created by Jacob Andersen on 8/5/25.
//

#include "PingRequestPacket.h"

#include "src/net/Connection.h"
#include "src/net/status/clientbound/PongResponsePacket.h"

namespace celerity::net::status::server {
void PingRequestPacket::handle(const PingRequestPacket& packet, Connection& connection) {
  connection.send_packet(client::PongResponsePacket{packet.timestamp_});
  connection.close();
}
}  // namespace celerity::net::status::server