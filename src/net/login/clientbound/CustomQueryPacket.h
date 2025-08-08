#ifndef CELERITY_NET_LOGIN_PACKETLOGINOUTLOGINPLUGINREQUEST_H
#define CELERITY_NET_LOGIN_PACKETLOGINOUTLOGINPLUGINREQUEST_H

#include <utility>

#include "src/net/Packet.h"

namespace celerity::net::login::client {
struct CustomQueryPacket final : Packet {
  int32_t message_id_;
  std::string channel_;
  std::vector<uint8_t> payload_;

  CustomQueryPacket(const int32_t message_id, const std::string &channel, const std::vector<uint8_t> &payload)
      : message_id_(message_id), channel_(channel), payload_(payload) {}

  ByteBuffer encode() const;
};
}  // namespace celerity::net::login::client

#endif
