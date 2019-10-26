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
  auto T_lenght = std::ceil((float)((float) pseudo_random_key.size() / (float)len));
  auto info_size = info.size();
  std::vector<uint8_t> T_last;
  std::vector<uint8_t> T;
   
  for(auto i = 0; i <= T_lenght; i++)
  {
    if(i != 0)
    { 
      std::vector<uint8_t> temp_for_T (info_size + T_last.size());
      memcpy(&temp_for_T[T_last.size()], &info[0], info_size);
      memcpy(&temp_for_T[0], &T_last[0], T_last.size());
      temp_for_T.push_back(i);


      hmac HMAC_SHA(pseudo_random_key.data(), pseudo_random_key.size());
      HMAC_SHA.update(pseudo_random_key.data(), pseudo_random_key.size());
      T_last.clear();

      for(size_t it = 0; it < HMAC_SHA.digest().size(); it++)
      {
        T.push_back(HMAC_SHA.digest().data()[it]);
        T_last.push_back(HMAC_SHA.digest().data()[it]);
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

std::vector<uint8_t> hkdf::expand_label(const std::string& label,
                                        const std::vector<uint8_t>& context, size_t length)
{
  /// \todo Implement HKDF-Expand-Label from TLS.
  return std::vector<uint8_t>();

}

std::vector<uint8_t> hkdf::derive_secret(const std::string& label,
                                         const std::vector<uint8_t>& messages)
{
  /// \todo Implement Derive-Secret from TLS.
  return std::vector<uint8_t>();

}


