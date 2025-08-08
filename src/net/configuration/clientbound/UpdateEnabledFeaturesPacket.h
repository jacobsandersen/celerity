//
// Created by Jacob Andersen on 7/4/25.
//

#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONOUTFEATUREFLAGS_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONOUTFEATUREFLAGS_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {
struct UpdateEnabledFeaturesPacket final : Packet {
  std::vector<std::string> m_features;

  explicit UpdateEnabledFeaturesPacket(const std::vector<std::string> &m_features) : m_features(m_features) {}

  [[nodiscard]] ByteBuffer encode() const;
};
}  // namespace celerity::net::configuration::client

#endif
