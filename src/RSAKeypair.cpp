#include "RSAKeypair.h"

#include <iostream>

namespace celerity {
CryptoPP::RSA::PrivateKey RSAKeypair::get_private_key() const {
  return m_private_key;
}

CryptoPP::RSA::PublicKey RSAKeypair::get_public_key() const {
  return m_public_key;
}

std::vector<uint8_t> RSAKeypair::get_der_encoded_public_key() const {
  std::vector<uint8_t> encodedPublicKey;

  CryptoPP::VectorSink vs(encodedPublicKey);
  get_public_key().DEREncode(vs);
  vs.MessageEnd();

  return encodedPublicKey;
}

std::vector<uint8_t> RSAKeypair::decrypt(
    const std::vector<uint8_t> &encrypted) const {
  std::vector<uint8_t> decrypted;

  const CryptoPP::RSAES_PKCS1v15_Decryptor decryptor(get_private_key());

  const size_t max_decrypted_size = decryptor.MaxPlaintextLength(encrypted.size());
  decrypted.resize(max_decrypted_size);

  CryptoPP::AutoSeededRandomPool rng;

  auto sink = CryptoPP::ArraySink(decrypted.data(), decrypted.size());
  auto filter = CryptoPP::PK_DecryptorFilter(rng, decryptor, &sink);
  CryptoPP::ArraySource src(encrypted.data(), encrypted.size(), true, &filter);

  return decrypted;
}
}  // namespace celerity