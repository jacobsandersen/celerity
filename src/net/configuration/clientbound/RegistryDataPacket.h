//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef REGISTRYDATAPACKET_H
#define REGISTRYDATAPACKET_H

#include "src/nbt/tag/Tag.h"
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {

struct RegistryEntry {
  std::string entry_id_;
  std::optional<std::unique_ptr<nbt::tag::Tag>> &data_;
};

struct RegistryDataPacket final : Packet {
  std::string registry_id_;
  std::vector<RegistryEntry> entries_;

  RegistryDataPacket(const std::string &registry_id, const std::vector<RegistryEntry> &entries)
      : registry_id_(registry_id), entries_(entries) {}

  ByteBuffer encode() const;
};

}  // namespace celerity::net::configuration::client

#endif  // REGISTRYDATAPACKET_H
