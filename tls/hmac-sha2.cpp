#include "hmac-sha2.h"
#include <iostream>



hmac_sha2::hmac_sha2(const uint8_t* key, std::size_t keysize)
{
  /// \todo Initialze with given key.
    std::vector<uint8_t>function_hashing(block_size);
    sha2 hash_function;
    if(keysize > block_size)
    {

        hash_function.update(key, keysize);
        auto data_hash = hash_function.digest().data();

        memcpy(&function_hashing[0], data_hash, digest_size);

        for(uint8_t k = digest_size; k < block_size; k++)
        {

            function_hashing[k] = firstIndex;
        }

    }
    else
    {
        memcpy(&function_hashing[firstIndex], key, keysize);

        for(uint8_t k = keysize; k < block_size; k++)
        {
            function_hashing[k] = firstIndex;

        }
    }

    for(uint8_t k = 0; k < block_size; k++)
    {
        I_padding_origin.push_back(function_hashing[k] ^ 0x36);



        O_padding_origin.push_back(function_hashing[k] ^ 0x5c);


    }
    

}

void hmac_sha2::update(const uint8_t* bytes, std::size_t size)
{
    std::vector<uint8_t> digest_vector(block_size + size);
    switch(flag)
    {
        case true:

            memcpy(&digest_vector[0], &I_padding_origin[0], block_size);
            memcpy(&digest_vector[block_size], bytes, size);

            hash_for_message.update(digest_vector.data(), digest_vector.size());

            flag = false;
            break;

        default:
        hash_for_message.update(bytes, size);
    }

}

hmac_sha2::digest_storage hmac_sha2::digest()
{

    digest_storage digest_final;
    digest_final = hash_for_message.digest();
    auto size2 = hash_for_message.digest().size();
    std::vector<uint8_t> final_vector(block_size + size2);

    memcpy(&final_vector[0], &O_padding_origin[0], block_size);
    memcpy(&final_vector[block_size], digest_final.data(), digest_final.size());

    auto data_output = final_vector.data();
    auto size_final = final_vector.size();
    hash_for_output.update(data_output, size_final );
  /// \todo Finalize HMAC compuation and return computed digest.
  auto hash_ret = hash_for_output.digest();
  return hash_ret;
}
