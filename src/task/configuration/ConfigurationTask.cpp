//
// Created by Jacob Andersen on 8/9/25.
//

#include "ConfigurationTask.h"

#include "src/net/configuration/clientbound/CustomPayloadPacket.h"
#include "src/net/configuration/clientbound/SelectKnownPacksPacket.h"
#include "src/net/configuration/clientbound/UpdateEnabledFeaturesPacket.h"

namespace celerity::task::configuration {
void ConfigurationTask::run() const {
  LOG(INFO) << "Sending server brand...";
  ByteBuffer brand;
  brand.write_string("celerity");
  connection_.send_packet(net::configuration::client::CustomPayloadPacket("minecraft:brand", brand.get_bytes()));

  LOG(INFO) << "Sending feature flags...";
  connection_.send_packet(net::configuration::client::UpdateEnabledFeaturesPacket({"minecraft:vanilla"}));

  LOG(INFO) << "Sending server's known packs...";
  connection_.send_packet(net::configuration::client::SelectKnownPacksPacket({{"minecraft", "core", "1.21.5"}}));
}
}  // namespace celerity::task::configuration