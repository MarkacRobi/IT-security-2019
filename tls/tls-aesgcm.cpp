    #include "tls-aesgcm.h"
#include "endian.h"

#include "aes128gcm.h"
#include <iostream>


#define FTEEN 14
#define POWTWO 2
#define BUFFER 16
#define TLS 0x03
#define ADDER 0xff
#define SUBT 8


tls13_aesgcm::tls13_aesgcm(const key_storage& key, const std::vector<uint8_t>& nonce_data) : nonce_(nonce_data), aes_(key)
{
}

tls13_aesgcm::~tls13_aesgcm() {}

std::vector<uint8_t> tls13_aesgcm::initializeData(record record_, 
                                  std::vector<uint8_t> &plaintext_,
                                  content_type type)
{
    std::vector<uint8_t> data;

  data.push_back(record_.header.type);


  data.push_back(record_.header.version.major);

  data.push_back(record_.header.version.minor);
  plaintext_.push_back(type);
  uint16_t ptSize = plaintext_.size();
  data.push_back(((uint16_t) aes128gcm::ciphertext_size(ptSize)>> SUBT)& ADDER);
  data.push_back((uint16_t) aes128gcm::ciphertext_size(ptSize) & ADDER);
  return data;

}


tls13_aesgcm::record tls13_aesgcm::encrypt(content_type type, const std::vector<uint8_t>& plaintext)
{
  std::vector<uint8_t> plaintext__(plaintext);
  bool testing = true;
  record record_;
record_.header.type = TLS_APPLICATION_DATA;
    record_.header.version = TLSv1_2;
    std::vector<uint8_t> data = initializeData(record_, plaintext__, type);


      aes_.encrypt(record_.ciphertext, plaintext__, nonce_.nonce(), data);
    uint16_t sizeOfPT = record_.ciphertext.size();
  record_.header.length = sizeOfPT;
  nonce_.operator ++();

  if(testing)
    return record_;
    else
    {
      /////
    }
    
}

bool tls13_aesgcm::decrypt(const record& record, std::vector<uint8_t>& plaintext,
                           content_type& type)
{


 std::vector<uint8_t> data;


  data.push_back(record.header.type);
  data.push_back(record.header.version.major);
  data.push_back(record.header.version.minor);
  
  data.push_back(record.header.length >> SUBT);
  data.push_back(record.header.length & ADDER);
  
  bool decrypt = aes_.decrypt(plaintext, record.ciphertext, nonce_.nonce(), data);
  
  if(decrypt == false)
  {
    return decrypt;
  }

  type = (content_type) plaintext.back();
  plaintext.pop_back();
 
  nonce_.operator ++();
  return decrypt;

}
