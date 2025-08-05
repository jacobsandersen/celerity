//
// Created by Jacob Andersen on 8/5/25.
//

#ifndef PINGRESPONSEPACKET_H
#define PINGRESPONSEPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::status {
struct PongResponsePacket final : Packet {
  int64_t timestamp_;

  explicit PongResponsePacket(const int64_t timestamp) : timestamp_(timestamp) {}

  ByteBuffer encode() const;
};
}  // namespace celerity::net::status

#endif  // PINGRESPONSEPACKET_H
