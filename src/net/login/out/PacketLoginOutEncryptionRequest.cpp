#include "PacketLoginOutEncryptionRequest.h"

namespace celerity::net::login {
void PacketLoginOutEncryptionRequest::write_data(ByteBuffer& buffer) {
  if (server_id.length() > 20) {
    buffer.write_string(server_id.substr(0, 20));
  } else {
    buffer.write_string(server_id);
  }

  buffer.write_varint(public_key_length);
  buffer.write_ubytes(public_key_bytes, public_key_length);
  buffer.write_varint(verify_token_length);
  buffer.write_ubytes(verify_token_bytes, verify_token_length);
  buffer.write_boolean(should_authenticate);
}
}  // namespace celerity::net::login
