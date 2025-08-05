//
// Created by Jacob Andersen on 7/26/25.
//

#ifndef CELERITY_BUFFERCRYPTER_H
#define CELERITY_BUFFERCRYPTER_H

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>

#include "ByteBuffer.h"
#include "SharedConstants.h"

namespace celerity {
class BufferCrypter {
 public:
  explicit BufferCrypter(const std::vector<uint8_t>& shared_secret) {
    m_shared_secret = CryptoPP::SecByteBlock(shared_secret.data(), shared_secret.size());
    m_aes = new CryptoPP::AES::Encryption(m_shared_secret, SHARED_SECRET_SIZE);
    m_enc_cipher = CryptoPP::CFB_Mode_ExternalCipher::Encryption(*m_aes, m_shared_secret, 1);
    m_dec_cipher = CryptoPP::CFB_Mode_ExternalCipher::Decryption(*m_aes, m_shared_secret, 1);
  }

  ~BufferCrypter() { delete m_aes; }

  ByteBuffer& encrypt(ByteBuffer& buffer);
  ByteBuffer& decrypt(ByteBuffer& buffer);

 private:
  CryptoPP::SecByteBlock m_shared_secret;
  CryptoPP::AES::Encryption* m_aes;
  CryptoPP::CFB_Mode_ExternalCipher::Encryption m_enc_cipher{};
  CryptoPP::CFB_Mode_ExternalCipher::Decryption m_dec_cipher{};
};
}  // namespace celerity

#endif
