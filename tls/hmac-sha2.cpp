#include "hmac-sha2.h"
#include <iostream>

hmac_sha2::hmac_sha2(const uint8_t* key, std::size_t keysize)
{
  /// \todo Initialze with given key.
  int firstIndex = 0;
  std::vector<uint8_t> hasher(block_size);

  if(block_size > keysize)
  {
      memcpy(&hasher[firstIndex], key, keysize);
      for(size_t k = digest_size; k<digest_size; k++)
      {
          hasher[k] = firstIndex;
      }
  }
  else if(keysize > block_size)
  {
      sha2 function_for_hashing;
      function_for_hashing.update(key, keysize);
      memcpy(&hasher[firstIndex], function_for_hashing.digest().data(), digest_size);

      for(size_t k = digest_size; k < block_size; k++)
      {
          hash[k] = firstIndex;
      }

  }

    for(size_t k = firstIndex; i < block_size; i++)
    {
        padding_I.push_back(hasher[k] ^ 0x36);

        padding_O.push_back(hasher[k] ^ 0x5c);
    }
}

void hmac_sha2::update(const uint8_t* bytes, std::size_t size)
{
  /// \todo Feed data to HMAC.
}

hmac_sha2::digest_storage hmac_sha2::digest()
{
  /// \todo Finalize HMAC compuation and return computed digest.
  return digest_storage();
}
