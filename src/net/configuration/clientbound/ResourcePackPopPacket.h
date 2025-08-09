//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef RESOURCEPACKPOPPACKET_H
#define RESOURCEPACKPOPPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {

struct ResourcePackPopPacket final : Packet {
  std::optional<uuids::uuid> id_;

  ResourcePackPopPacket() : ResourcePackPopPacket(std::nullopt) {}

  explicit ResourcePackPopPacket(const std::optional<uuids::uuid> &id) : id_(id) {}

  ByteBuffer encode() const;
};

}  // namespace celerity::net::configuration::client

#endif  // RESOURCEPACKPOPPACKET_H
