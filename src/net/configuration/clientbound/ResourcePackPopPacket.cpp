//
// Created by Jacob Andersen on 8/9/25.
//

#include "ResourcePackPopPacket.h"

namespace celerity::net::configuration::client {
ByteBuffer ResourcePackPopPacket::encode() const {
  ByteBuffer buf;

  if (!id_) {
    buf.write_boolean(false);
    return buf;
  }

  buf.write_boolean(true);
  buf.write_uuid(*id_);

  return buf;
}
}  // namespace celerity::net::configuration::client