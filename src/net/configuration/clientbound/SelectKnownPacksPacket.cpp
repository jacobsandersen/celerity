//
// Created by Jacob Andersen on 7/4/25.
//

#include "SelectKnownPacksPacket.h"

namespace celerity::net::configuration::client {
ByteBuffer SelectKnownPacksPacket::encode() const {
  ByteBuffer buffer;
  buffer.write_varint(m_known_packs.size());
  for (const auto& known_pack : m_known_packs) {
    buffer.write_string(known_pack.get_namespace());
    buffer.write_string(known_pack.get_id());
    buffer.write_string(known_pack.get_version());
  }
  return buffer;
}
}  // namespace celerity::net::configuration::client
