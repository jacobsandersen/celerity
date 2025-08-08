#include "KeepAlivePacket.h"

namespace celerity::net::configuration::client {
ByteBuffer KeepAlivePacket::encode() const {
  ByteBuffer buf;
  buf.write_be_long(payload_);
  return buf;
}
}  // namespace celerity::net::configuration::client
