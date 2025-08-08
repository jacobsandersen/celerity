//
// Created by Jacob Andersen on 8/6/25.
//

#include "CustomPayloadPacket.h"

namespace celerity::net::configuration::client {
ByteBuffer CustomPayloadPacket::encode() const {
  ByteBuffer buf;
  buf.write_string(channel_);
  buf.write_ubytes(payload_.data(), payload_.size());
  return buf;
}
}  // namespace celerity::net::configuration::client