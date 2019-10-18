#include "hkdf.h"


hkdf::hkdf(const std::vector<uint8_t>& salt, const std::vector<uint8_t>& ikm)
{
/// \todo initialize based on salt and ikm using HKDF-Extract

  /*hmac HMAC(salt.data(), salt.size());
	HMAC.update(ikm.data(), ikm.size());
	hmac_sha2::digest_storage  dig = HMAC.digest();

	for(size_t i = 0; i < dig.size(); i++)
		_prk.push_back(dig.data()[i]); */


  hmac HMAC_SHA(salt.data(), salt.size());
  HMAC_SHA.update(ikm.data(), ikm.size());

  for (uint8_t i = 0; i < HMAC_SHA.digest().size(); i++)
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
  return std::vector<uint8_t>();

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


