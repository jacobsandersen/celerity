//
// Created by Jacob Andersen on 8/9/25.
//

#include "RegistryDataTask.h"

#include "src/MinecraftServer.h"
#include "src/net/configuration/clientbound/RegistryDataPacket.h"

namespace celerity::task::configuration {
void RegistryDataTask::run() const {
  LOG(INFO) << "Running RegistryDataTask";
  for (auto& registry : MinecraftServer::get_server().get_registries()) {
    LOG(INFO) << "Sending registry: " + registry.get_registry_id().to_string();
    connection_.send_packet(net::configuration::client::RegistryDataPacket(registry));
  }
}
}  // namespace celerity::task::configuration