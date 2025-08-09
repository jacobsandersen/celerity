//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef COOKIERESPONSEPACKET_H
#define COOKIERESPONSEPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::server {

struct CookieResponsePacket final : Packet {
  std::string key_;
  std::vector<uint8_t> payload_;

  explicit CookieResponsePacket(ByteBuffer buffer)
      : key_(buffer.read_string()), payload_(buffer.read_ubytes(buffer.get_data_length())) {}

  static void handle(const CookieResponsePacket& packet, Connection& connection);
};

}  // namespace celerity::net::configuration::server

#endif  // COOKIERESPONSEPACKET_H
