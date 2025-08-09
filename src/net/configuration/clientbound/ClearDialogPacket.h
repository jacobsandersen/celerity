//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef CLEARDIALOGPACKET_H
#define CLEARDIALOGPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {

struct ClearDialogPacket final : Packet {
  ByteBuffer encode() const;
};

}  // namespace celerity::net::configuration::client

#endif  // CLEARDIALOGPACKET_H
