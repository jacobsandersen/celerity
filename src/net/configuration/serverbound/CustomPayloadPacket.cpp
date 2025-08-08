#include "CustomPayloadPacket.h"

#include <absl/log/log.h>

namespace celerity::net::configuration::server {
void CustomPayloadPacket::handle(const CustomPayloadPacket& packet, Connection& connection) {
  // TODO: this is for plugins later, need better api
  if (packet.channel_ == "minecraft:brand") {
    ByteBuffer buf(packet.payload_);
    LOG(INFO) << "Client brand: " << buf.read_string();
  }
}
}  // namespace celerity::net::configuration::server
