#include "CustomQueryPacket.h"

namespace celerity::net::login::client {
ByteBuffer CustomQueryPacket::encode() const {
  ByteBuffer buf;

  buf.write_varint(message_id_);
  buf.write_string(channel_);
  buf.write_ubytes(payload_.data(), payload_.size());

  return buf;
}
}  // namespace celerity::net::login::client
