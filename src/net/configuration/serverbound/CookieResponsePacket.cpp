//
// Created by Jacob Andersen on 8/9/25.
//

#include "CookieResponsePacket.h"

#include <absl/log/log.h>

namespace celerity::net::configuration::server {
void CookieResponsePacket::handle(const CookieResponsePacket& packet, Connection& connection) {
  // TODO: this will be part of server transfer handling later
  LOG(INFO) << "Received cookie response packet";
}
}  // namespace celerity::net::configuration::server