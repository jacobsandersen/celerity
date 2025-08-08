//
// Created by Jacob Andersen on 7/4/25.
//

#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINKNOWNPACKS_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINKNOWNPACKS_H

#include "src/KnownPack.h"
#include "src/net/Packet.h"

namespace celerity::net::configuration::server {
struct SelectKnownPacksPacket final : Packet {
  std::vector<KnownPack> known_packs_{};

  explicit SelectKnownPacksPacket(ByteBuffer buffer) {
    const auto length = buffer.read_varint();
    if (length == 0) return;

    for (int i = 0; i < length; ++i) {
      const auto ns = buffer.read_string();
      const auto id = buffer.read_string();
      const auto version = buffer.read_string();
      known_packs_.emplace_back(ns, id, version);
    }
  }

  static void handle(const SelectKnownPacksPacket& packet, Connection& connection);
};
}  // namespace celerity::net::configuration::server

#endif
