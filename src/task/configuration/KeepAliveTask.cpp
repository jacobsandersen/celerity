//
// Created by Jacob Andersen on 8/9/25.
//

#include "KeepAliveTask.h"

#include "KeepAliveKillerTask.h"
#include "src/MinecraftServer.h"
#include "src/net/configuration/clientbound/KeepAlivePacket.h"

namespace celerity::task::configuration {
void KeepAliveTask::run() const {
  LOG(INFO) << "Keep alive task running";

  const auto keep_alive_payload = std::time(nullptr);
  connection_.set_context_value("keep_alive_payload", keep_alive_payload);

  LOG(INFO) << "Generated payload " << keep_alive_payload;

  const auto keep_alive_killer_id = connection_.get_scheduler()->schedule_async_task(
      std::chrono::seconds(30), std::make_unique<KeepAliveKillerTask>(connection_));
  connection_.set_context_value("keep_alive_killer", keep_alive_killer_id);

  LOG(INFO) << "Created killer task and sending packet";

  connection_.send_packet(net::configuration::client::KeepAlivePacket(keep_alive_payload));
}
}  // namespace celerity::task::configuration