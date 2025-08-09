//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef TRANSFERPACKET_H
#define TRANSFERPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {
struct TransferPacket final : Packet {
  std::string host_;
  int32_t port_;

  TransferPacket(const std::string &host, const int32_t port) : host_(host), port_(port) {}

  ByteBuffer encode() const;
};
}  // namespace celerity::net::configuration::client

#endif  // TRANSFERPACKET_H
