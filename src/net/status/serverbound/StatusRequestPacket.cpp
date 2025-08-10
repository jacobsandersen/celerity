//
// Created by Jacob Andersen on 8/5/25.
//

#include "StatusRequestPacket.h"

#include "../clientbound/StatusResponsePacket.h"
#include "src/MinecraftServer.h"
#include "src/net/Connection.h"

namespace celerity::net::status::server {
void StatusRequestPacket::handle(const StatusRequestPacket& packet, Connection& connection) {
  LOG(INFO) << "received status request";
  if (connection.has_context_value("status_requested")) {
    LOG(INFO) << "status already requested";
    return;
  }


  LOG(INFO) << "saving request";
  connection.set_context_value("status_requested", true);

  MinecraftServer& server = MinecraftServer::get_server();
  const auto server_config = server.get_config_manager().get_server_config();

  const auto max_players = server_config.get_max_players();
  const auto motd = server_config.get_motd();
  const auto favicon = server_config.get_favicon();

  LOG(INFO) << "building response packet";
  const auto response = client::StatusResponsePacket{server.get_version_name(),
                                                     server.get_protocol_version(),
                                                     max_players,
                                                     static_cast<uint16_t>(server.get_players().size()),
                                                     {},
                                                     motd,
                                                     favicon,
                                                     false};

  LOG(INFO) << "sending response";
  connection.send_packet(response);
}
}  // namespace celerity::net::status::server