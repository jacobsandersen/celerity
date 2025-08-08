//
// Created by simple on 11/17/23.
//

#ifndef CELERITY_NET_LOGIN_PACKETLOGININLOGINSTART_H
#define CELERITY_NET_LOGIN_PACKETLOGININLOGINSTART_H

#include "src/net/Packet.h"

namespace celerity::net::login::server {
struct HelloPacket final : Packet {
  std::string username_;
  uuids::uuid unique_id_;

  explicit HelloPacket(ByteBuffer buf) : username_(buf.read_string()), unique_id_(buf.read_uuid()) {}

  static void handle(const HelloPacket& packet, Connection& connection);
};
}  // namespace celerity::net::login::server

#endif
