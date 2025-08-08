//
// Created by Jacob Andersen on 7/26/25.
//

#include "BufferCrypter.h"

#include <cryptopp/filters.h>

namespace celerity {
ByteBuffer& BufferCrypter::encrypt(ByteBuffer& buffer) {
  const auto bytes = buffer.get_bytes();

  buffer.reset();

  auto enc_bytes = std::vector<uint8_t>{};
  CryptoPP::VectorSource src(
      bytes, true, new CryptoPP::StreamTransformationFilter(m_enc_cipher, new CryptoPP::VectorSink(enc_bytes)));
  buffer.set_bytes(enc_bytes);

  return buffer;
}

ByteBuffer& BufferCrypter::decrypt(ByteBuffer& buffer) {
  const auto enc_bytes = buffer.get_bytes();

  buffer.reset();

  auto bytes = std::vector<uint8_t>{};
  CryptoPP::VectorSource src(enc_bytes, true,
                             new CryptoPP::StreamTransformationFilter(m_dec_cipher, new CryptoPP::VectorSink(bytes)));
  buffer.set_bytes(bytes);

  return buffer;
}
}  // namespace celerity
