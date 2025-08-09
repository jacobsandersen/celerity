//
// Created by Jacob Andersen on 8/9/25.
//

#include "ResourcePackPushPacket.h"

namespace celerity::net::configuration::client {
ByteBuffer ResourcePackPushPacket::encode() const {
  ByteBuffer buf;
  buf.write_uuid(id_);
  buf.write_string(url_);
  buf.write_string(hash_);
  buf.write_boolean(forced_);
  if (prompt_message_) {
    buf.write_boolean(true);
    buf.write_string(*prompt_message_);
  } else {
    buf.write_boolean(false);
  }
  return buf;
}
}  // namespace celerity::net::configuration::client