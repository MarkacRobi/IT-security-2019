#include "tls-isap.h"
#include "isap128.h"
#include "endian.h"

tls13_isap::tls13_isap(const key_storage& key, const std::vector<uint8_t>& nonce_data)
{
  // \todo Initialize with given key and nonce
}

tls13_isap::~tls13_isap() {}

tls13_isap::record tls13_isap::encrypt(content_type type, const std::vector<uint8_t>& plaintext)
{
  /// \todo Implement ciphertext record generation for given plaintext.
  return record();
}

bool tls13_isap::decrypt(const record& record, std::vector<uint8_t>& plaintext, content_type& type)
{
  /// \todo Implement decryption for the given record.
  return false;
}
