//
// Created by Jacob Andersen on 8/9/25.
//

#include "RegistryDataPacket.h"

#include "src/nbt/NBTWriter.h"

namespace celerity::net::configuration::client {
ByteBuffer RegistryDataPacket::encode() const {
  ByteBuffer buf;
  registry_.write(buf);
  return buf;
}
}  // namespace celerity::net::configuration::client