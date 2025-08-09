//
// Created by Jacob Andersen on 8/9/25.
//

#include "ShowDialogPacket.h"

#include "src/nbt/NBTWriter.h"

namespace celerity::net::configuration::client {
ByteBuffer ShowDialogPacket::encode() const {
  ByteBuffer buf;
  const nbt::NBTWriter writer(buf);
  writer.write_tag(dialog_);
  return buf;
}
}  // namespace celerity::net::configuration::client