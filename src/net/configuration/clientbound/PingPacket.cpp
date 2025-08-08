//
// Created by Jacob Andersen on 8/8/25.
//

#include "PingPacket.h"

namespace celerity::net::configuration::client {
ByteBuffer PingPacket::encode() const {
  ByteBuffer buf;
  buf.write_be_int(id_);
  return buf;
}
}  // namespace celerity::net::configuration::client