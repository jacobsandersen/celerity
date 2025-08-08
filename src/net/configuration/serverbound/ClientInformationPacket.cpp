#include "ClientInformationPacket.h"

#include <absl/log/log.h>

#include "../../../MinecraftServer.h"

namespace celerity::net::configuration::server {
void ClientInformationPacket::handle(const ClientInformationPacket& packet, Connection& connection) {
  const auto player = MinecraftServer::get_server()->get_player(connection.get_unique_id());
  if (player == nullptr) {
    connection.send_disconnection("Could not get Player instance for your UUID");
    return;
  }

  player->set_client_information(std::make_unique<player::ClientInformation>(packet.information_));
}
}  // namespace celerity::net::configuration::server
