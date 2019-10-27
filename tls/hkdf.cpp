#include "hkdf.h"
#include <cmath>
#include <iostream>

hkdf::hkdf(const std::vector<uint8_t>& salt, const std::vector<uint8_t>& ikm)
{
/// \todo initialize based on salt and ikm using HKDF-Extract

  uint8_t salt_size = salt.size();
  hmac HMAC_SHA(salt.data(), salt_size);

  uint8_t ikm_size = ikm.size();
  HMAC_SHA.update(ikm.data(), ikm_size);

  for (size_t i = 0; i < HMAC_SHA.digest().size(); i++)
      pseudo_random_key.push_back(HMAC_SHA.digest().data()[i]);

    
}

hkdf::hkdf(const std::vector<uint8_t>& prk)
{
  //// \todo initialize based on the given PRK
  pseudo_random_key = prk;
}

std::vector<uint8_t> hkdf::expand(const std::vector<uint8_t>& info, size_t len)
{ 
  //// \todo Return HKDF-Expand for given info and length
  auto n_length = std::ceil((float)((float) pseudo_random_key.size() / (float)len));
  auto info_size = info.size();
  std::vector<uint8_t> T_last;
  std::vector<uint8_t> T;
   
  for(auto i = 0; i <= n_length; i++)
  {
    if(i != 0)
    { 
      auto vector_size = info_size + T_last.size();
      std::vector<uint8_t> temp_for_T (vector_size);
      memcpy(&temp_for_T[T_last.size()], &info[0], info_size);
      memcpy(&temp_for_T[0], &T_last[0], T_last.size());
      temp_for_T.push_back(i);


      hmac HMAC_SHA(pseudo_random_key.data(), pseudo_random_key.size());
      HMAC_SHA.update(temp_for_T.data(), temp_for_T.size());
      T_last.clear();

      for(size_t it = 0; it < HMAC_SHA.digest().size(); it++)
      { 
        T_last.push_back(HMAC_SHA.digest().data()[it]);
        T.push_back(HMAC_SHA.digest().data()[it]);
      }

    }
  }

  return trunc_to_l_bytes(T, len);

}

std::vector<uint8_t> hkdf::trunc_to_l_bytes(const std::vector<uint8_t>& T, size_t len)
{
  std::vector<uint8_t> hkfd_expand;
  for(size_t i = 0; i < len; i++)
  {
    hkfd_expand.push_back(T[i]);
  }
  return hkfd_expand;
}


std::string hkdf::getConcatened(const std::string& label)
{
  return label + "tls13 ";
}

std::vector<uint8_t> hkdf::expand_label(const std::string& label,
                                        const std::vector<uint8_t>& context, size_t length)
{
  /// \todo Implement HKDF-Expand-Label from TLS.

  uint8_t firstByte = length & getFirstByte;
  uint8_t secondByte = length & getSecondByte; 
  std::string tls_label_concatened = getConcatened(label);
  std::vector<uint8_t> hkdf_expand_label;
  hkdf_expand_label.push_back(firstByte);
  hkdf_expand_label.push_back(secondByte);
  hkdf_expand_label.push_back(tls_label_concatened.size());



  size_t i = 0;
  while(i < tls_label_concatened.size())
  {
    hkdf_expand_label.push_back(tls_label_concatened[i]);
    i++;
  }


  hkdf_expand_label.push_back(context.size());
  size_t j = 0;
  while(j < context.size())
  {
    hkdf_expand_label.push_back(context[j]);
    j++;
  }
  
  std::vector<uint8_t>return_hkd_expand_label = expand(hkdf_expand_label, length);

  return return_hkd_expand_label;

}

std::vector<uint8_t> hkdf::derive_secret(const std::string& label,
                                         const std::vector<uint8_t>& messages)
{
  /// \todo Implement Derive-Secret from TLS.
  sha2 function;
  function.update(messages.data(), messages.size());
  std::vector<uint8_t> message;

  size_t i = 0;
  while(i < function.digest().size())
  {
    message.push_back(function.digest().data()[i]);
    i++;
  }

  std::vector<uint8_t> derive_secret = expand_label(label, message, message.size());
  return derive_secret;

}


