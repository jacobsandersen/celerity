//
// Created by Jacob Andersen on 8/6/25.
//

#ifndef COOKIEREQUESTPACKET_H
#define COOKIEREQUESTPACKET_H
#include <utility>

#include "src/net/Packet.h"

namespace celerity::net::configuration::client {

struct CookieRequestPacket final : Packet {
  std::string key_;

  explicit CookieRequestPacket(std::string key) : key_(std::move(key)) {}

  [[nodiscard]] ByteBuffer encode() const;
};

}  // namespace celerity::net::configuration::client

#endif  // COOKIEREQUESTPACKET_H
