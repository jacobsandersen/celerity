//
// Created by Jacob Andersen on 8/9/25.
//

#include "FinishConfigurationPacket.h"

#include <absl/log/log.h>

#include "src/net/Connection.h"

namespace celerity::net::configuration::server {
void FinishConfigurationPacket::handle([[maybe_unused]] const FinishConfigurationPacket& packet, Connection& connection) {
  LOG(INFO) << "Client acknowledges finish configuration, switching to Play";
  connection.set_state(ConnectionState::kPlay);
}
}  // namespace celerity::net::configuration::server