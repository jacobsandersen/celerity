//
// Created by Jacob Andersen on 8/9/25.
//

#include "StoreCookiePacket.h"

namespace celerity::net::configuration::client {
ByteBuffer StoreCookiePacket::encode() const {
  ByteBuffer buf;
  buf.write_string(key_);
  buf.write_varint(payload_.size());
  buf.write_ubytes(payload_);
  return buf;
}
}  // namespace celerity::net::configuration::client