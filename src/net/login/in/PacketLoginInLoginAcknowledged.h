#ifndef CELERITY_NET_LOGIN_PACKETLOGININLOGINACKNOWLEDGED_H
#define CELERITY_NET_LOGIN_PACKETLOGININLOGINACKNOWLEDGED_H

#include "../../../ByteBuffer.h"
#include "../../Connection.h"
#include "../../Packet.h"

namespace celerity::net::login {
class PacketLoginInLoginAcknowledged : public Packet {
 public:
  void handle(const std::shared_ptr<Connection>&, const std::unique_ptr<ByteBuffer>&) override;
};
}  // namespace celerity::net::login

#endif
