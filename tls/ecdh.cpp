#include "ecdh.h"

#include "../ecclib/gfp/gfp.h"
#include "../ecclib/protocols/ecdh.h"
#include "../ecclib/protocols/eckeygen.h"
#include "../ecclib/utils/param.h"
#include "../ecclib/utils/rand.h"
#include "endian.h"
#include "random.h"

using namespace std;

#define param (&param_)

ecdh::ecdh(const curve_type_t type)
{
  // Load curve data
  param_load(&param_, type);
}

void ecdh::generate_key_pair()
{
  // Generate key pair
  eckeygen(private_key_, &public_key_, &param_);
}

void ecdh::set_private_key(const gfp_t private_key)
{
  // Copy private key
  gfp_copy(private_key_, private_key);
  // Compute public key
  ecdh_phase_one(&public_key_, private_key_, &param_);
}

std::vector<uint8_t> ecdh::get_data() const
{
  vector<uint8_t> data;

  data.push_back(hton((uint8_t) 4 ));

  for(size_t i = 0; i < 8; i++) {
      for(int j = 3; j >= 0; j--) {
          data.push_back((public_key_.x[i] >> (j * 8u) & 0xff));
      }
  }

    for(size_t i = 0; i < 8; i++) {
        for(int j = 3; j >= 0; j--) {
            data.push_back((public_key_.y[i] >> (j * 8u) & 0xff));
        }
    }

    return data;
}

std::vector<uint8_t> ecdh::get_shared_secret(const std::vector<uint8_t>& other_party_data) const
{
    /// Decode second public key and run phase 2 of ECDH. Return the shared secret.
    eccp_point_affine_t shared2;
    vector<uint8_t> data;
    eccp_point_affine_t other_party_data2;
    int nr = 0;

    for(size_t i = 0; i < 8; i++) {
      nr = (int) other_party_data.at((i * 4) + 1) << 24;
      nr += (int) other_party_data.at((i * 4) + 2) << 16;
      nr += (int) other_party_data.at((i * 4) + 3) << 8;
      nr += (int) other_party_data.at((i * 4) + 4);
      other_party_data2.x[i] = nr;
    }

    for(size_t i = 8; i < 16; i++){
        nr= (int) other_party_data.at((i * 4) + 1) << 24;
        nr += (int) other_party_data.at((i * 4) + 2) << 16;
        nr += (int) other_party_data.at((i * 4) + 3) << 8;
        nr += (int) other_party_data.at((i * 4) + 4);
        other_party_data2.y[i - 8] = nr;
    }

    ecdh_phase_two(&shared2, private_key_, &other_party_data2, &param_);

    for(size_t i = 0; i < 8; i++) {
        for(int j = 3; j >= 0; j--) {
            data.push_back((shared2.x[i] >> (j * 8) & 0xff));
        }
    }

    return data;
}
