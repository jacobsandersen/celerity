//
// Created by Jacob Andersen on 8/9/25.
//

#include "PongPacket.h"

#include "src/net/Connection.h"

namespace celerity::net::configuration::server {
void PongPacket::handle(const PongPacket& packet, Connection& connection) {
  const auto maybe_last_id = connection.get_context_value("last_pong_id");
  if (!maybe_last_id) {
    connection.send_disconnection("Received pong but did not ask for it");
    return;
  }

  try {
    if (packet.id_ != boost::any_cast<int32_t>(*maybe_last_id)) {
      connection.send_disconnection("Pong ID did not match my Ping ID");
    }
  } catch (boost::bad_any_cast&) {}
}
}  // namespace celerity::net::configuration::server