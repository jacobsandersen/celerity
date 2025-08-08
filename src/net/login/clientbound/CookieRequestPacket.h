//
// Created by Jacob Andersen on 8/6/25.
//

#ifndef COOKIEREQUESTPACKET_H
#define COOKIEREQUESTPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::login::client {

struct CookieRequestPacket final : Packet {
  std::string key_;

  explicit CookieRequestPacket(const std::string &key) : key_(key) {}

  ByteBuffer encode() const;
};

}  // namespace celerity::net::login::client

#endif  // COOKIEREQUESTPACKET_H
