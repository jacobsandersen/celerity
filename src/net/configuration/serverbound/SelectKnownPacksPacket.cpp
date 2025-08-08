//
// Created by Jacob Andersen on 7/4/25.
//

#include "SelectKnownPacksPacket.h"

#include "src/net/Connection.h"

namespace celerity::net::configuration::server {
void SelectKnownPacksPacket::handle(const SelectKnownPacksPacket& packet, Connection& connection) {
  connection.set_known_packs(packet.known_packs_);
}
}  // namespace celerity::net::configuration::server
