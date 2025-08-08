//
// Created by Jacob Andersen on 8/6/25.
//

#ifndef FINISHCONFIGURATIONPACKET_H
#define FINISHCONFIGURATIONPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {
struct FinishConfigurationPacket final : Packet {
  explicit FinishConfigurationPacket([[maybe_unused]] ByteBuffer buffer) {}

  [[nodiscard]] ByteBuffer encode() const;
};
}  // namespace celerity::net::configuration::client

#endif  // FINISHCONFIGURATIONPACKET_H
