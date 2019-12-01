#include "tls-handshake-client.h"

#include "hkdf.h"
#include "random.h"

#define LEGACY_STANDARD_VERSION 0x0303
#define ONE 1
#define ZERO 0
#define EIGHT 8
#define NULA 0x00
#define DVAJES_TRI 0x17
#define HEX_FF 0xff

tls_handshake_client::tls_handshake_client(tls_record_layer& layer, const psk_map& psks)
  : layer_(layer), psks_(psks), ecdh_(SECP256R1), fixed_randomness_({{0}}),
    have_fixed_randomness_(false)
{
  ecdh_.generate_key_pair();
}

tls_handshake_client::tls_handshake_client(tls_record_layer& layer,
                                           const std::map<std::string, std::vector<uint8_t>>& psks,
                                           const random_struct& fixed_randomness,
                                           const gfp_t& ecdh_private)
  : layer_(layer), psks_(psks), ecdh_(SECP256R1), fixed_randomness_(fixed_randomness),
    have_fixed_randomness_(true)
{
  ecdh_.set_private_key(ecdh_private);
}


alert_location tls_handshake_client::start_handshake(const std::vector<std::string>& psk_identities)
{
  //TODO:
  for (const std::string& psk_identity : psk_identities)
  {
    if (psks_.find(psk_identity) == psks_.end())
      return {local, internal_error};
  }
  psk_identities_ = psk_identities;

  alert_location alert;
  // send ClientHello
  send_client_hello();
  // read ServerHello
  alert = read_server_hello();
  if (!alert)
    return alert;
  // read Finished
  alert = read_finished();
  if (!alert)
    return alert;
  // write Finished
  send_finished();
  return {local, ok};
}

void tls_handshake_client::send_client_hello()
{
/// \todo: Send ClientHello message
/// If have_fixed_randomness_ is false generate random data.
/// If it is true, use fixed_randomness_ as random data. In this case it is always 32 byte
/// large.

//Init header message 
HandshakePayload handshake_payload_client_hello_msg; 
Extension extension_types_version, extension_types_group, extension_type_share_key, extension_types_psk_key, extension_type_pre_shared_key; 
KeyShareEntry entry_share_key; 
size_t size_of_client_hello_message, size_of_extention; 
handshake_message_header handshake_message_header_;
std::vector<uint8_t> vector_ecdh; 
OfferedPsks offeredPSKS; 

size_of_client_hello_message = 0;
size_of_extention = 0; 
//Set legacy standard version  
handshake_payload_client_hello_msg.legacy_version = LEGACY_STANDARD_VERSION; 
size_of_client_hello_message = size_of_client_hello_message + 2; 
handshake_message_header_.msg_type = handshake_types::CLIENT_HELLO; 
vector_ecdh =ecdh_.get_data();
size_t size_of_vector_ecdh = vector_ecdh.size();

if(!have_fixed_randomness_)
{
  get_random_data(reinterpret_cast<uint8_t *> (handshake_payload_client_hello_msg.random.random_bytes), 32);

}
else
{

  std::memcpy(&handshake_payload_client_hello_msg.random.random_bytes, &fixed_randomness_, 32);

}
//incerement client msg size
size_of_client_hello_message = size_of_client_hello_message + 32; 

//push back to legacy session id
handshake_payload_client_hello_msg.legacy_session_id.push_back(ZERO);
cipher_suite cipher_suite__; 
for (cipher_suite cipher_suite_ : layer_.get_supported_cipher_suites())
{

  //push back cipher suites
  handshake_payload_client_hello_msg.cipher_suites.push_back(cipher_suite_);
}

handshake_payload_client_hello_msg.legacy_compression_methods.push_back(ONE);
size_of_client_hello_message = size_of_client_hello_message + 6; 

extension_types_version.type = ExtensionType::SUPPORTED_VERSIONS; 
extension_types_group.type = ExtensionType::SUPPORTED_GROUPS;
extension_type_share_key.type = ExtensionType::KEY_SHARE; 
extension_types_psk_key.type = ExtensionType::PSK_KEY_EXCHANGE_MODES;
extension_type_pre_shared_key.type = ExtensionType::PRE_SHARED_KEY; 

entry_share_key.group = NamedGroup::SECP_256_R1;

//TODO: check
auto tls_major = TLSv1_3.major;
auto tls_minot = TLSv1_3.minor;
extension_types_version.data.push_back(tls_major);
extension_types_version.data.push_back(tls_minot);
//inceremnet size of extention
size_of_extention = size_of_extention + 7; 


extension_types_group.data.push_back(NULA);
extension_types_group.data.push_back(DVAJES_TRI);
//inceremnet size of extention
size_of_extention = size_of_extention + 8; 

extension_type_share_key.data.resize(size_of_vector_ecdh + 4);

extension_type_share_key.data[0] = (entry_share_key.group>>(ONE * EIGHT));
extension_type_share_key.data[0] &= HEX_FF;
extension_type_share_key.data[1] = (entry_share_key.group>>(ZERO * EIGHT));
extension_type_share_key.data[1] &= HEX_FF;
extension_type_share_key.data[2] = (size_of_vector_ecdh>>(ONE * EIGHT));
extension_type_share_key.data[2] &= HEX_FF;
extension_type_share_key.data[3] = (size_of_vector_ecdh>>(ZERO* EIGHT));
extension_type_share_key.data[3] &= HEX_FF;


memcpy(&extension_type_share_key.data[4], &vector_ecdh[0], size_of_vector_ecdh);
//inceremnet size of extention
size_of_extention = size_of_extention + extension_type_share_key.data.size() + 6;

extension_types_psk_key.data.push_back(PskKeyExchangeMode::PSK_DHE_KE);
//inceremnet size of extention
size_of_extention = size_of_extention + 6;

for(std::string id : psk_identities_)
{
  //make new object of psk
  PskIdentity psk_identity; 
  size_t size_of_data_key; 
  size_t size_of_id = id.size();
  size_of_data_key = extension_type_pre_shared_key.data.size();

  psk_identity.identity = id;
  
  psk_identity.obfuscated_ticket_age = 0; 

  offeredPSKS.identities.push_back(psk_identity);
  
  extension_type_pre_shared_key.data.resize(size_of_data_key + size_of_id + 6);

  extension_type_pre_shared_key.data[size_of_data_key] = (size_of_id >> (ONE * EIGHT));
  extension_type_pre_shared_key.data[size_of_data_key] &= HEX_FF;

  extension_type_pre_shared_key.data[size_of_data_key + ONE] = (size_of_id >> (ZERO * EIGHT));
  extension_type_pre_shared_key.data[size_of_data_key + ONE] &= HEX_FF;
  size_t index = 0; 
  while(index < size_of_id)
  {
    extension_type_pre_shared_key.data[size_of_data_key + index + 2] = (uint8_t) id[index];
    index++;
  }
  extension_type_pre_shared_key.data[size_of_data_key + size_of_id + 3] = ZERO;
  extension_type_pre_shared_key.data[size_of_data_key + size_of_id + 4] = ZERO;
  extension_type_pre_shared_key.data[size_of_data_key + size_of_id + 5] = ZERO;
  extension_type_pre_shared_key.data[size_of_data_key + size_of_id + 6] = ZERO;


}
//inceremnet size of extention
size_of_extention = size_of_extention + extension_type_pre_shared_key.data.size() + 6;

handshake_payload_client_hello_msg.extensions.push_back(extension_type_share_key);

handshake_payload_client_hello_msg.extensions.push_back(extension_types_version);

handshake_payload_client_hello_msg.extensions.push_back(extension_types_psk_key);

handshake_payload_client_hello_msg.extensions.push_back(extension_types_group);

handshake_payload_client_hello_msg.extensions.push_back(extension_type_pre_shared_key);


}

alert_location tls_handshake_client::read_server_hello()
{
  /// \todo Read and handle ServerHello message
  return {local, internal_error};
}

alert_location tls_handshake_client::read_finished()
{
  /// \todo Read and handle Finished message
  return {local, internal_error};
}

void tls_handshake_client::send_finished()
{
/// \todo Send Finished message.
}

