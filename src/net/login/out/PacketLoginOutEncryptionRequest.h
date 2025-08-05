#ifndef CELERITY_NET_LOGIN_PACKETLOGINOUTENCRYPTIONREQUEST_H
#define CELERITY_NET_LOGIN_PACKETLOGINOUTENCRYPTIONREQUEST_H

#include <memory>
#include <string>
#include <utility>

#include "../../Connection.h"
#include "../../OutboundPacket.h"

namespace celerity::net::login {
class PacketLoginOutEncryptionRequest : public OutboundPacket {
 public:
  PacketLoginOutEncryptionRequest(std::string server_id, int32_t public_key_length, uint8_t* public_key_bytes,
                                  int32_t verify_token_length, uint8_t* verify_token_bytes, bool should_authenticate)
      : OutboundPacket(0x01),
        server_id(std::move(server_id)),
        public_key_length(public_key_length),
        public_key_bytes(public_key_bytes),
        verify_token_length(verify_token_length),
        verify_token_bytes(verify_token_bytes),
        should_authenticate(should_authenticate) {}

  void write_data(ByteBuffer& buffer) override;

 private:
  std::string server_id;
  int32_t public_key_length;
  uint8_t* public_key_bytes;
  int32_t verify_token_length;
  uint8_t* verify_token_bytes;
  bool should_authenticate;
};
}  // namespace celerity::net::login
#endif
