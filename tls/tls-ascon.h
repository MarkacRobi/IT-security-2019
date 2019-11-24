#ifndef TLS_ASCON_H
#define TLS_ASCON_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "tls-cipher.h"
#include "./ascon128.h"
#include "./counter.h"


/// Oracle simulating application data record encryption and decryption for
/// TLS 1.3 using ASCON as cipher suite.
class tls13_ascon : public tls13_cipher
{
private:
    ascon128 ascon;
    incrementing_nonce nonce;
public:
  /// Instantiate with given Ascon key and nonce generator.
  tls13_ascon(const key_storage& key, const std::vector<uint8_t>& nonce_data);
    bool decrypt(const record& record, std::vector<uint8_t>& plaintext, content_type& type);

  record encrypt(content_type type, const std::vector<uint8_t>& plaintext);
  virtual ~tls13_ascon();
  std::vector<uint8_t> addAdditionalData(record new_record, std::vector<uint8_t> vec);


};

#endif
