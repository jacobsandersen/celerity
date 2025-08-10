#include "KeepAlivePacket.h"

#include "src/MinecraftServer.h"
#include "src/net/Connection.h"
#include "src/net/configuration/clientbound/KeepAlivePacket.h"
#include "src/task/configuration/KeepAliveTask.h"

namespace celerity::net::configuration::server {
void KeepAlivePacket::handle(const KeepAlivePacket& packet, Connection& connection) {
  LOG(INFO) << "Received keep alive response";

  if (const auto maybe_killer_id = connection.get_context_value("keep_alive_killer")) {
    try {
      const auto killer_id = boost::any_cast<uuids::uuid>(*maybe_killer_id);
      connection.get_scheduler()->cancel(killer_id);
      LOG(INFO) << "Cancelled previous killer task";
    } catch (boost::bad_any_cast&) {
    }
  }

  const auto maybe_last_payload = connection.get_context_value("keep_alive_payload");
  if (!maybe_last_payload) {
    connection.send_disconnection("Keep alive received, but I didn't ask for it");
    return;
  }

  try {
    if (const auto last_payload = boost::any_cast<int64_t>(*maybe_last_payload); last_payload != packet.payload_) {
      connection.send_disconnection("Keep alive payload did not match last sent payload");
      return;
    }
  } catch (boost::bad_any_cast&) {
  }

  LOG(INFO) << "Client's keep alive response OK. Scheduling new keep alive in 15 seconds";

  connection.get_scheduler()->schedule_async_task(std::chrono::seconds(15),
                                                  std::make_unique<task::configuration::KeepAliveTask>(connection));
}
}  // namespace celerity::net::configuration::server
