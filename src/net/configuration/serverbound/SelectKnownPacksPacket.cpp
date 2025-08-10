//
// Created by Jacob Andersen on 7/4/25.
//

#include "SelectKnownPacksPacket.h"

#include "src/MinecraftServer.h"
#include "src/net/Connection.h"
#include "src/task/configuration/RegistryDataTask.h"

namespace celerity::net::configuration::server {
void SelectKnownPacksPacket::handle(const SelectKnownPacksPacket& packet, Connection& connection) {
  LOG(INFO) << "Received client known packs...";
  connection.set_known_packs(packet.known_packs_);

  LOG(INFO) << "Starting registry data task...";
  connection.get_scheduler()->run_async_task(std::make_unique<task::configuration::RegistryDataTask>(connection));
}
}  // namespace celerity::net::configuration::server
