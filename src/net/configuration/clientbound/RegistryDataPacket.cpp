//
// Created by Jacob Andersen on 8/9/25.
//

#include "RegistryDataPacket.h"

#include "src/nbt/NBTWriter.h"

namespace celerity::net::configuration::client {
ByteBuffer RegistryDataPacket::encode() const {
  ByteBuffer buf;
  const nbt::NBTWriter nbt_writer(buf);

  buf.write_string(registry_id_);
  buf.write_varint(entries_.size());
  for (const auto& [entry_id_, data_] : entries_) {
    buf.write_string(entry_id_);

    if (!data_) {
      buf.write_boolean(false);
      continue;
    }

    buf.write_boolean(true);
    nbt_writer.write_tag(*data_);
  }

  return buf;
}
}  // namespace celerity::net::configuration::client