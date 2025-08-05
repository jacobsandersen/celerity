//
// Created by Jacob Andersen on 7/4/25.
//

#include "PacketConfigurationInKnownPacks.h"

#include <glog/logging.h>

#include "../../../KnownPack.h"

namespace celerity::net::configuration {
void PacketConfigurationInKnownPacks::handle(const std::shared_ptr<Connection>& conn,
                                             const std::unique_ptr<ByteBuffer>& buffer) {
  LOG(INFO) << "Received server known packs...";

  std::vector<KnownPack> known_packs;

  const auto length = buffer->read_varint();
  if (length == 0) {
    LOG(INFO) << "Client did not send any packs.";
  }

  for (int i = 0; i < length; i++) {
    const auto ns = buffer->read_string();
    const auto id = buffer->read_string();
    const auto version = buffer->read_string();
    known_packs.emplace_back(ns, id, version);

    LOG(INFO) << "Client knows about pack " << ns << ":" << id << " with version " << version;
  }

  conn->set_known_packs(std::move(known_packs));

  LOG(INFO) << "Continuing configuration process from here.";
}
}  // namespace celerity::net::configuration
