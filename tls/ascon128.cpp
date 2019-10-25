#include "ascon128.h"
#include "../ascon/crypto_aead.h"

ascon128::ascon128(){
/// \todo Initialize with an all 0 key.
}

ascon128::ascon128(const key_storage& key)
{
  key_holder =key;
  /// \todo Initialize with given key.
}

void ascon128::set_key(const key_storage& key)
{
  key_holder=key;
  /// \todo Reset key
}

bool ascon128::encrypt(std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& plaintext,
                       const std::vector<uint8_t>& nonce_data,
                       const std::vector<uint8_t>& additional_data) const
{
  /// \todo Encrypt data using Ascon with the given nonce and additional data.
   unsigned long long crypto_holder = 0;
     auto pl_size = plaintext.size();

  unsigned char* crypto_array = new unsigned char[pl_size + CRYPTO_KEYBYTES];

  auto pl_data = plaintext.data();

  auto ad_data = additional_data.data();
  auto ad_size = additional_data.size();

  unsigned char* casted_nonce_data = (unsigned char*)nonce_data.data();
  auto kh_data = key_holder.data();

  crypto_aead_encrypt(crypto_array, &crypto_holder, pl_data, 
                pl_size, ad_data , ad_size, 
                NULL, casted_nonce_data, kh_data);

  ciphertext.resize(crypto_holder);


  
  memcpy(&ciphertext[0],
   crypto_array, pl_size + CRYPTO_KEYBYTES);



  return true;
}

bool ascon128::decrypt(std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& ciphertext,
                       const std::vector<uint8_t>& nonce_data,
                       const std::vector<uint8_t>& additional_data) const
{
  unsigned long long ull = 0;
  int ct_size = ciphertext.size();
  unsigned char* array_chara = new unsigned char[ct_size - CRYPTO_KEYBYTES];

  auto ct_data = ciphertext.data();

  auto ad_data = additional_data.data();
  auto ad_size = additional_data.size();

  unsigned char* casted_nonce_data = (unsigned char*)nonce_data.data();
  auto kh_data = key_holder.data();

  uint8_t crypt_decrypt_value = crypto_aead_decrypt(array_chara, &ull, NULL, ct_data, ct_size, 
                                  ad_data,ad_size, 
                                  casted_nonce_data, kh_data);
  plaintext.resize(ull);
  memcpy(plaintext.data(),
  array_chara, ull);

  if(crypt_decrypt_value == CORRECT)
    return true;
  else
  {
    return false;
  }
  
  

}
