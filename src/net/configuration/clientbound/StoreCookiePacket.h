//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef STORECOOKIEPACKET_H
#define STORECOOKIEPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {

struct StoreCookiePacket final : Packet {
  std::string key_;
  std::vector<uint8_t> payload_;

  StoreCookiePacket(const std::string &key, const std::vector<uint8_t> &payload) : key_(key), payload_(payload) {}

  ByteBuffer encode() const;
};

}  // namespace celerity::net::configuration::client

#endif  // STORECOOKIEPACKET_H
