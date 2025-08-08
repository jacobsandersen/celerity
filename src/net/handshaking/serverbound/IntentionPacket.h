//
// Created by simple on 11/10/23.
//

#ifndef CELERITY_NET_HANDSHAKING_INTENTIONPACKET_H
#define CELERITY_NET_HANDSHAKING_INTENTIONPACKET_H

#include <json/config.h>

#include <string>

#include "src/ByteBuffer.h"
#include "src/net/Packet.h"

namespace celerity::net::handshaking::server {
enum class ClientIntention { Status = 1, Login = 2, Transfer = 3 };

struct IntentionPacket final : Packet {
  const int32_t protocol_version_;
  const std::string server_address_;
  const uint16_t server_port_;
  const ClientIntention next_state_;

  explicit IntentionPacket(ByteBuffer& buffer)
      : protocol_version_(buffer.read_varint()),
        server_address_(buffer.read_string()),
        server_port_(buffer.read_be_short()),
        next_state_(static_cast<ClientIntention>(buffer.read_varint())) {}

  static void handle(const IntentionPacket& packet, Connection& connection);
};
}  // namespace celerity::net::handshaking::server

#endif
