#include "HelloPacket.h"

namespace celerity::net::login::client {
ByteBuffer HelloPacket::encode() const {
  ByteBuffer buf;

  if (server_id_.length() > 20) {
    buf.write_string(server_id_.substr(0, 20));
  } else {
    buf.write_string(server_id_);
  }

  buf.write_varint(public_key_length_);
  buf.write_ubytes(public_key_bytes_, public_key_length_);
  buf.write_varint(verify_token_length_);
  buf.write_ubytes(verify_token_bytes_, verify_token_length_);
  buf.write_boolean(should_authenticate_);

  return buf;
}
}  // namespace celerity::net::login::client
