//
// Created by simple on 11/17/23.
//

#ifndef CELERITY_NET_LOGIN_PACKETLOGININENCRYPTIONRESPONSE_H
#define CELERITY_NET_LOGIN_PACKETLOGININENCRYPTIONRESPONSE_H

#include "src/net/Packet.h"

namespace celerity::net::login::server {
struct KeyPacket final : Packet {
  std::vector<uint8_t> encrypted_shared_secret_;
  std::vector<uint8_t> encrypted_verify_token_;

  explicit KeyPacket(ByteBuffer& buffer) {
    const auto encrypted_shared_secret_length = buffer.read_varint();
    encrypted_shared_secret_ = buffer.read_ubytes(encrypted_shared_secret_length);

    const auto encrypted_verify_token_length = buffer.read_varint();
    encrypted_verify_token_ = buffer.read_ubytes(encrypted_verify_token_length);
  }

  static void handle(const KeyPacket& packet, Connection& connection);
};
}  // namespace celerity::net::login::server

#endif
