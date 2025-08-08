#ifndef CELERITY_NET_LOGIN_PACKETLOGININLOGINACKNOWLEDGED_H
#define CELERITY_NET_LOGIN_PACKETLOGININLOGINACKNOWLEDGED_H
#include "src/net/Packet.h"

namespace celerity::net::login::server {
struct LoginAcknowledgedPacket final : Packet {
  explicit LoginAcknowledgedPacket([[maybe_unused]] ByteBuffer buffer) {};

  static void handle(const LoginAcknowledgedPacket& packet, Connection& connection);
};
}  // namespace celerity::net::login::server

#endif
