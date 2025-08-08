#include "LoginAcknowledgedPacket.h"

#include <absl/log/log.h>

#include "src/net/Connection.h"
#include "src/net/configuration/clientbound/KeepAlivePacket.h"
#include "src/net/configuration/clientbound/SelectKnownPacksPacket.h"
#include "src/net/configuration/clientbound/UpdateEnabledFeaturesPacket.h"

namespace celerity::net::login::server {
void LoginAcknowledgedPacket::handle([[maybe_unused]] const LoginAcknowledgedPacket& packet, Connection& connection) {
  LOG(INFO) << "Client acknowledges login, switching to configuration state";

  connection.set_state(ConnectionState::kConfiguration);

  LOG(INFO) << "Starting configuration keep alive";
  const auto keep_alive_payload = std::time(nullptr);
  const auto keep_alive_killer_id = connection.get_scheduler().schedule_task(std::chrono::seconds(30), [&connection] {
    connection.send_disconnection("Client did not respond to keep alive");
  });

  connection.send_packet(configuration::client::KeepAlivePacket(keep_alive_payload));

  connection.set_context_value("keep_alive_payload", keep_alive_payload);
  connection.set_context_value("keep_alive_killer", keep_alive_killer_id);

  LOG(INFO) << "Sending feature flags...";
  connection.send_packet(configuration::client::UpdateEnabledFeaturesPacket({"minecraft:vanilla"}));

  LOG(INFO) << "Sending server's known packs...";
  connection.send_packet(configuration::client::SelectKnownPacksPacket({{"minecraft", "core", "1.21.5"}}));
}
}  // namespace celerity::net::login::server
