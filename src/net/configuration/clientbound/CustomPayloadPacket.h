//
// Created by Jacob Andersen on 8/6/25.
//

#ifndef CUSTOMPAYLOADPACKET_H
#define CUSTOMPAYLOADPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {

struct CustomPayloadPacket final : Packet {
  std::string channel_;
  std::vector<uint8_t> payload_;

  CustomPayloadPacket(const std::string &channel, const std::vector<uint8_t> &payload)
      : channel_(channel), payload_(payload) {}

  [[nodiscard]] ByteBuffer encode() const;
};

}  // namespace celerity::net::configuration::client

#endif  // CUSTOMPAYLOADPACKET_H
