//
// Created by Jacob Andersen on 8/9/25.
//

#include "CustomClickActionPacket.h"

#include <absl/log/log.h>

namespace celerity::net::configuration::server {
void CustomClickActionPacket::handle(const CustomClickActionPacket& packet, Connection& connection) {
  LOG(INFO) << "Received custom click action with ID " << packet.id_;
}
}  // namespace celerity::net::configuration::server