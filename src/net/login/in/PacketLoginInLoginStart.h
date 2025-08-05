//
// Created by simple on 11/17/23.
//

#ifndef CELERITY_NET_LOGIN_PACKETLOGININLOGINSTART_H
#define CELERITY_NET_LOGIN_PACKETLOGININLOGINSTART_H

#include <memory>

#include "../../../ByteBuffer.h"
#include "../../Connection.h"
#include "../../Packet.h"

namespace celerity::net::login {
class PacketLoginInLoginStart : public Packet {
 public:
  void handle(const std::shared_ptr<Connection>&,
              const std::unique_ptr<ByteBuffer>&) override;
};
}  // namespace celerity::net::login

#endif
