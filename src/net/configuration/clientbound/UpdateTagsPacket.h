//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef UPDATETAGSPACKET_H
#define UPDATETAGSPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {
struct TagEntry {
  std::string registry_;
  std::unordered_map<std::string, std::vector<int32_t>> entries;
};

struct UpdateTagsPacket final : Packet {
  std::vector<TagEntry> tags_;

  explicit UpdateTagsPacket(const std::vector<TagEntry> &tags) : tags_(tags) {}

  ByteBuffer encode() const;
};
}  // namespace celerity::net::configuration::client

#endif  // UPDATETAGSPACKET_H
