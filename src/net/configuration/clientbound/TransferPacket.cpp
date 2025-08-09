//
// Created by Jacob Andersen on 8/9/25.
//

#include "TransferPacket.h"

namespace celerity::net::configuration::client {
ByteBuffer TransferPacket::encode() const {
  ByteBuffer buf;
  buf.write_string(host_);
  buf.write_varint(port_);
  return buf;
}
}  // namespace celerity::net::configuration::client