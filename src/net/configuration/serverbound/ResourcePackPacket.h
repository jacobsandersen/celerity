//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef RESOURCEPACKPACKET_H
#define RESOURCEPACKPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::server {
enum class ResourcePackResult {
  SuccessfullyDownloaded,
  Declined,
  FailedToDownload,
  Accepted,
  Downloaded,
  InvalidUrl,
  FailedToReload,
  Discarded
};

struct ResourcePackPacket final : Packet {
  uuids::uuid id_;
  ResourcePackResult result_;

  explicit ResourcePackPacket(ByteBuffer buffer)
      : id_(buffer.read_uuid()), result_(static_cast<ResourcePackResult>(buffer.read_varint())) {}

  static void handle(const ResourcePackPacket& result, Connection& connection);
};
}  // namespace celerity::net::configuration::server

#endif  // RESOURCEPACKPACKET_H
