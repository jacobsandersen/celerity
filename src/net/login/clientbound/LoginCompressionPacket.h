#ifndef CELERITY_NET_LOGIN_PACKETLOGINOUTSETCOMPRESSION_H
#define CELERITY_NET_LOGIN_PACKETLOGINOUTSETCOMPRESSION_H
#include "src/net/Packet.h"

namespace celerity::net::login::client {
struct LoginCompressionPacket final : Packet {
  int32_t threshold_;

  explicit LoginCompressionPacket(const int32_t threshold) : threshold_(threshold) {}

  ByteBuffer encode() const;
};
}  // namespace celerity::net::login::client
#endif
