//
// Created by Jacob Andersen on 8/5/25.
//

#include "PongResponsePacket.h"

namespace celerity::net::status::client {
ByteBuffer PongResponsePacket::encode() const {
  ByteBuffer buf;
  buf.write_be_long(timestamp_);

  return buf;
}
}  // namespace celerity::net::status::client