//
// Created by Jacob Andersen on 7/4/25.
//

#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONOUTKNOWNPACKS_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONOUTKNOWNPACKS_H

#include "../../../KnownPack.h"
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {
struct SelectKnownPacksPacket final : Packet {
  std::vector<KnownPack> m_known_packs;

  explicit SelectKnownPacksPacket(const std::vector<KnownPack> &m_known_packs) : m_known_packs(m_known_packs) {}

  [[nodiscard]] ByteBuffer encode() const;
};
}  // namespace celerity::net::configuration::client

#endif
