#ifndef TLS_ISAP_H
#define TLS_ISAP_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "tls-cipher.h"
#include "isap128.h"
#include "counter.h"

/// Oracle simulating application data record encryption and decryption for
/// TLS 1.3 using ISAP as cipher suite.
class tls13_isap : public tls13_cipher
{
private:
    isap128 isap;
    incrementing_nonce nonce;
public:
  /// Instantiate with given ISAP key and nonce generator.
  tls13_isap(const key_storage& key, const std::vector<uint8_t>& nonce_data);
  virtual ~tls13_isap();

  record encrypt(content_type type, const std::vector<uint8_t>& plaintext);
  bool decrypt(const record& record, std::vector<uint8_t>& plaintext, content_type& type);

    std::vector<uint8_t> addAdditionalData(record new_record, std::vector<uint8_t> vec);
};

#endif
