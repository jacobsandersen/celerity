#ifndef CELERITY_RSAKEYPAIR_H
#define CELERITY_RSAKEYPAIR_H

#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

namespace celerity {
constexpr uint32_t kRSAKeyPairSize = 1024;

class RSAKeypair {
 public:
  RSAKeypair() {
    CryptoPP::AutoSeededRandomPool pool;
    CryptoPP::InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(pool, kRSAKeyPairSize);
    m_private_key = CryptoPP::RSA::PrivateKey(params);
    m_public_key = CryptoPP::RSA::PublicKey(m_private_key);
  }

  [[nodiscard]] CryptoPP::RSA::PrivateKey get_private_key() const;

  [[nodiscard]] CryptoPP::RSA::PublicKey get_public_key() const;

  [[nodiscard]] std::vector<uint8_t> get_der_encoded_public_key() const;

  [[nodiscard]] std::vector<uint8_t> decrypt(const std::vector<uint8_t> &encrypted) const;

 private:
  CryptoPP::RSA::PrivateKey m_private_key;
  CryptoPP::RSA::PublicKey m_public_key;
};
}  // namespace celerity

#endif
