//
// Created by Jacob Andersen on 7/4/25.
//

#include "UpdateEnabledFeaturesPacket.h"

namespace celerity::net::configuration::client {
ByteBuffer UpdateEnabledFeaturesPacket::encode() const {
  ByteBuffer buffer;
  buffer.write_varint(m_features.size());
  for (auto& m_feature : m_features) {
    buffer.write_string(m_feature);
  }
  return buffer;
}
}  // namespace celerity::net::configuration::client
