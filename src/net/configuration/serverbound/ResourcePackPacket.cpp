//
// Created by Jacob Andersen on 8/9/25.
//

#include "ResourcePackPacket.h"

#include <absl/log/log.h>

namespace celerity::net::configuration::server {
void ResourcePackPacket::handle(const ResourcePackPacket& result, Connection& connection) {
  LOG(INFO) << "Received resource pack response";
}
}  // namespace celerity::net::configuration::server