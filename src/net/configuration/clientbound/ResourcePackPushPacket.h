//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef RESOURCEPACKPUSHPACKET_H
#define RESOURCEPACKPUSHPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {

struct ResourcePackPushPacket final : Packet {
  uuids::uuid id_;
  std::string url_;
  std::string hash_;
  bool forced_;
  std::optional<std::string> prompt_message_;

  ResourcePackPushPacket(const uuids::uuid &id, const std::string &url, const std::string &hash, const bool forced,
                         const std::optional<std::string> &prompt_message)
      : id_(id), url_(url), hash_(hash), forced_(forced), prompt_message_(prompt_message) {}

  ByteBuffer encode() const;
};

}  // namespace celerity::net::configuration::client

#endif  // RESOURCEPACKPUSHPACKET_H
