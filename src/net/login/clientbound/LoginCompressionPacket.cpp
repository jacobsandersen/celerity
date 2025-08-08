#include "LoginCompressionPacket.h"

namespace celerity::net::login::client {
ByteBuffer LoginCompressionPacket::encode() const {
  ByteBuffer buf;
  buf.write_varint(threshold_);
  return buf;
}
}  // namespace celerity::net::login::client
