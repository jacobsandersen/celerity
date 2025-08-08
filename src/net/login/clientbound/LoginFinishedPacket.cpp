#include "LoginFinishedPacket.h"

namespace celerity::net::login::client {
ByteBuffer LoginFinishedPacket::encode() const {
  ByteBuffer buf;

  buf.write_uuid(*unique_id_);
  buf.write_string(username_);
  buf.write_varint(static_cast<int32_t>(properties.size()));
  for (const auto &prop : properties) {
    buf.write_string(prop.name);
    buf.write_string(prop.value);
    buf.write_boolean(prop.is_signed());
    if (prop.is_signed()) {
      buf.write_string(prop.signature);
    }
  }

  return buf;
}
}  // namespace celerity::net::login::client
