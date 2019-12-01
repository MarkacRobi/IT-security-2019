#include "isap128.h"
#include "../isap/crypto_aead.h"

isap128::isap128(){
}

isap128::isap128(const key_storage& key)
{
  key_ = key;
}

void isap128::set_key(const key_storage& key)
{
  key_ = key;
}

bool isap128::encrypt(std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& plaintext,
                       const std::vector<uint8_t>& nonce_data,
                       const std::vector<uint8_t>& additional_data) const
{
  if (nonce_data.size() != nonce_size)
    return false;

  unsigned long long ct_size = 0;
  ciphertext.resize(ciphertext_size(plaintext.size()));
  crypto_aead_encrypt_isap(ciphertext.data(), &ct_size, plaintext.data(), plaintext.size(),
                      additional_data.data(), additional_data.size(), nullptr, nonce_data.data(),
                      key_.data());
  return true;
}

bool isap128::decrypt(std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& ciphertext,
                       const std::vector<uint8_t>& nonce_data,
                       const std::vector<uint8_t>& additional_data) const
{
  if (nonce_data.size() != nonce_size)
    return false;
  if (ciphertext.size() < additional_size)
    return false;

  unsigned long long pt_size = 0;
  plaintext.resize(plaintext_size(ciphertext.size()));
  const int ret = crypto_aead_decrypt_isap(plaintext.data(), &pt_size, nullptr, ciphertext.data(),
                                      ciphertext.size(), additional_data.data(),
                                      additional_data.size(), nonce_data.data(), key_.data());
  return (ret == 0);
}
