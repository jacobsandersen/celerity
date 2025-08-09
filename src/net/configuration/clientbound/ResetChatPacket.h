//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef RESETCHATPACKET_H
#define RESETCHATPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {

struct ResetChatPacket final : Packet {
  ByteBuffer encode() const;
};

}  // namespace celerity::net::configuration::client

#endif  // RESETCHATPACKET_H
