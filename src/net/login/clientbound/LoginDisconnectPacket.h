#ifndef CELERITY_NET_LOGIN_PACKETLOGINOUTDISCONNECT_H
#define CELERITY_NET_LOGIN_PACKETLOGINOUTDISCONNECT_H

#include <string>
#include <utility>

#include "src/net/Packet.h"

namespace celerity::net::login::client {
struct LoginDisconnectPacket final : Packet {
  std::string reason_;

  explicit LoginDisconnectPacket(std::string reason) : reason_(std::move(reason)) {}

  [[nodiscard]] ByteBuffer encode() const;
};
}  // namespace celerity::net::login::client

#endif
