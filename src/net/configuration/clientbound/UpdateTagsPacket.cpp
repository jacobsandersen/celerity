//
// Created by Jacob Andersen on 8/9/25.
//

#include "UpdateTagsPacket.h"

namespace celerity::net::configuration::client {
ByteBuffer UpdateTagsPacket::encode() const {
  ByteBuffer buf;
  buf.write_varint(tags_.size());
  for (const auto& [registry, entries] : tags_) {
    buf.write_string(registry);
    buf.write_varint(entries.size());
    for (const auto& [tag, payload] : entries) {
      buf.write_string(tag);
      buf.write_varint(payload.size());
      for (const int varint : payload) {
        buf.write_varint(varint);
      }
    }
  }
  return buf;
}
}  // namespace celerity::net::configuration::client