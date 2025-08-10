#include "LoginAcknowledgedPacket.h"

#include <absl/log/log.h>

#include "src/net/Connection.h"
#include "src/task/configuration/ConfigurationTask.h"
#include "src/task/configuration/KeepAliveTask.h"

namespace celerity::net::login::server {
void LoginAcknowledgedPacket::handle([[maybe_unused]] const LoginAcknowledgedPacket& packet, Connection& connection) {
  LOG(INFO) << "Client acknowledges login, switching to configuration state";
  connection.set_state(ConnectionState::kConfiguration);

  LOG(INFO) << "Starting configuration keep alive...";
  connection.get_scheduler()->run_async_task(std::make_unique<task::configuration::KeepAliveTask>(connection));

  LOG(INFO) << "Starting server configuration...";
  connection.get_scheduler()->run_async_task(std::make_unique<task::configuration::ConfigurationTask>(connection));
}
}  // namespace celerity::net::login::server
