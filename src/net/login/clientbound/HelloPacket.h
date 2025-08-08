#ifndef CELERITY_NET_LOGIN_PACKETLOGINOUTENCRYPTIONREQUEST_H
#define CELERITY_NET_LOGIN_PACKETLOGINOUTENCRYPTIONREQUEST_H

#include <string>
#include <utility>

#include "src/net/Packet.h"

namespace celerity::net::login::client {
struct HelloPacket final : Packet {
  std::string server_id_;
  int32_t public_key_length_;
  const uint8_t* public_key_bytes_;
  int32_t verify_token_length_;
  uint8_t* verify_token_bytes_;
  bool should_authenticate_;

  HelloPacket(std::string server_id, const int32_t public_key_length, const uint8_t* public_key_bytes,
              const int32_t verify_token_length, uint8_t* verify_token_bytes, const bool should_authenticate)
      : server_id_(std::move(server_id)),
        public_key_length_(public_key_length),
        public_key_bytes_(public_key_bytes),
        verify_token_length_(verify_token_length),
        verify_token_bytes_(verify_token_bytes),
        should_authenticate_(should_authenticate) {}

  ByteBuffer encode() const;
};
}  // namespace celerity::net::login::client
#endif
