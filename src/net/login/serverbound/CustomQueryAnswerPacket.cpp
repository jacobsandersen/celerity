#include "CustomQueryAnswerPacket.h"

#include <absl/log/log.h>

namespace celerity::net::login::server {
void CustomQueryAnswerPacket::handle(const CustomQueryAnswerPacket& packet, Connection& connection) {
  // TODO: this will be part of plugins or something; only relevant when a CustomQueryPacket was sent
  LOG(INFO) << "Received CustomQueryAnswer with message id " << packet.message_id_;
}
}  // namespace celerity::net::login::server
