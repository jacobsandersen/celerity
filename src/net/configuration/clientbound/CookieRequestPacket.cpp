//
// Created by Jacob Andersen on 8/6/25.
//

#include "CookieRequestPacket.h"

namespace celerity::net::configuration::client {
ByteBuffer CookieRequestPacket::encode() const {
  ByteBuffer buf;
  buf.write_string(key_);
  return buf;
}
}  // namespace celerity::net::configuration::client