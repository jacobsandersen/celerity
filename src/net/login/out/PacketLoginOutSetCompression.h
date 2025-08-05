#ifndef CELERITY_NET_LOGIN_PACKETLOGINOUTSETCOMPRESSION_H
#define CELERITY_NET_LOGIN_PACKETLOGINOUTSETCOMPRESSION_H

#include "../../OutboundPacket.h"

namespace celerity::net::login {
class PacketLoginOutSetCompression : public OutboundPacket {
 public:
  PacketLoginOutSetCompression() : OutboundPacket(0x03) {}

  void write_data(ByteBuffer& buffer) override;
};
}  // namespace celerity::net::login
#endif
