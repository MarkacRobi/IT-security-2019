#include "tls-handshake-client.h"

#include "hkdf.h"
#include "random.h"

#define LEGACY_STANDARD_VERSION 0x0303
#define TWO 2
#define ONE 1
#define THREE 3 
#define ZERO 0
#define EIGHT 8
#define NULA 0x00
#define DVAJES_TRI 0x17
#define HEX_FF 0xff
#define FOUR 4
#define SIXTEEN 16

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
size_t client_msg_len;
handshake_message_header handshake_message_header_;
std::vector<uint8_t> vector_ecdh; 
OfferedPsks offeredPSKS; 
std::vector<uint8_t> temp_data_from_handshake;
std::vector<uint8_t> vector_data_handshake;

size_of_client_hello_message = 0;
size_of_extention = 0; 
//Set legacy standard version  
handshake_payload_client_hello_msg.legacy_version = LEGACY_STANDARD_VERSION; 
size_of_client_hello_message = size_of_client_hello_message + 2; 
handshake_message_header_.msg_type = handshake_types::CLIENT_HELLO; 
vector_ecdh =ecdh_.get_data();
size_t size_of_vector_ecdh = vector_ecdh.size();
size_t size_of_hash;
size_t index_for_payload_handshake = 45;

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
    extension_type_pre_shared_key.data[size_of_data_key + index + TWO] = (uint8_t) id[index];
    index++;
  }
  extension_type_pre_shared_key.data[size_of_data_key + size_of_id + 3] = ZERO;
  extension_type_pre_shared_key.data[size_of_data_key + size_of_id + 4] = ZERO;
  extension_type_pre_shared_key.data[size_of_data_key + size_of_id + 5] = ZERO;
  extension_type_pre_shared_key.data[size_of_data_key + size_of_id + 6] = ZERO;


}
//inceremnet size of extention
size_of_extention = size_of_extention + extension_type_pre_shared_key.data.size() + 6;
std::vector<uint8_t> vector_handshake_payload_client_hello(size_of_client_hello_message+ size_of_extention+5);
size_of_hash = size_of_extention + 35;

handshake_payload_client_hello_msg.extensions.push_back(extension_type_share_key);

handshake_payload_client_hello_msg.extensions.push_back(extension_types_version);

handshake_payload_client_hello_msg.extensions.push_back(extension_types_psk_key);

handshake_payload_client_hello_msg.extensions.push_back(extension_types_group);

handshake_payload_client_hello_msg.extensions.push_back(extension_type_pre_shared_key);


std::memcpy(&vector_handshake_payload_client_hello[ZERO], 
            &handshake_payload_client_hello_msg.legacy_version,TWO);
std::memcpy(&vector_handshake_payload_client_hello[TWO], 
            &handshake_payload_client_hello_msg.random.random_bytes, 32);
std::memcpy(&vector_handshake_payload_client_hello[34], 
            &handshake_payload_client_hello_msg.legacy_session_id[ZERO], ONE);

vector_handshake_payload_client_hello[35] = (uint8_t)(FOUR >> EIGHT);
vector_handshake_payload_client_hello[35] &= HEX_FF;
vector_handshake_payload_client_hello[36] = (uint8_t)(FOUR & HEX_FF);

std::memcpy(&vector_handshake_payload_client_hello[37], 
            &handshake_payload_client_hello_msg.cipher_suites[ZERO], FOUR);
std::memcpy(&vector_handshake_payload_client_hello[41], 
            &handshake_payload_client_hello_msg.legacy_compression_methods[ZERO], ONE);
 

vector_handshake_payload_client_hello[42] = (uint8_t) (size_of_hash >> SIXTEEN);
vector_handshake_payload_client_hello[42] &= HEX_FF;
vector_handshake_payload_client_hello[43] = (uint8_t) (size_of_hash >> EIGHT);
vector_handshake_payload_client_hello[43] &= HEX_FF;
vector_handshake_payload_client_hello[44] = (uint8_t) (size_of_hash & HEX_FF);


size_t index_client_msg = 0; 
size_t size_of_client_extension = handshake_payload_client_hello_msg.extensions.size();
uint16_t size_of_pre_key;
uint16_t size_temp;
uint16_t size_of_temp;
uint16_t size_temp_; 
while(index_client_msg < size_of_client_extension)
{
  Extension temp_extension;
  temp_extension = handshake_payload_client_hello_msg.extensions[index_client_msg];    
  //TODO: STart
  vector_handshake_payload_client_hello[index_for_payload_handshake] = (temp_extension.type >> (ONE * EIGHT)); 
  
  vector_handshake_payload_client_hello[index_for_payload_handshake] &= HEX_FF;
  
  vector_handshake_payload_client_hello[index_for_payload_handshake + ONE] = (temp_extension.type >> (ZERO * EIGHT));
  
  vector_handshake_payload_client_hello[index_for_payload_handshake + ONE] &= HEX_FF;

  if(index_client_msg == ONE || index_client_msg == TWO)
  {
    size_temp_ = (uint16_t) temp_extension.data.size() + ONE;
   
    vector_handshake_payload_client_hello[index_for_payload_handshake + TWO] = (size_temp_ >> (ONE * EIGHT));
    vector_handshake_payload_client_hello[index_for_payload_handshake + TWO] &= HEX_FF;
    
    vector_handshake_payload_client_hello[index_for_payload_handshake + 3] = (size_temp_ >> (ZERO * EIGHT));
    vector_handshake_payload_client_hello[index_for_payload_handshake + 3] &= HEX_FF;

    if(index_client_msg == ONE)
    {
      vector_handshake_payload_client_hello[index_for_payload_handshake + 4] = TWO;

    }
      
    else if(index_client_msg == TWO)
    {
      vector_handshake_payload_client_hello[index_for_payload_handshake + 4] = ONE;
    }
      
    index_for_payload_handshake = index_for_payload_handshake + 5;
  }
  else if(index_client_msg == 3)
  {
    size_of_temp = 4;
    
    vector_handshake_payload_client_hello[index_for_payload_handshake + 2] = (size_of_temp >> (ONE * EIGHT));
    vector_handshake_payload_client_hello[index_for_payload_handshake + 2] &= HEX_FF;
    
    vector_handshake_payload_client_hello[index_for_payload_handshake + 3] = (size_of_temp >> (ZERO * EIGHT));
    vector_handshake_payload_client_hello[index_for_payload_handshake + 3] &= HEX_FF;
    
    size_of_temp = size_of_temp - TWO;
    
    vector_handshake_payload_client_hello[index_for_payload_handshake + 4] = (size_of_temp >> (ONE * EIGHT));
    vector_handshake_payload_client_hello[index_for_payload_handshake + 4] &= HEX_FF;
    
    vector_handshake_payload_client_hello[index_for_payload_handshake + 5] = (size_of_temp >> (ZERO * EIGHT));
    vector_handshake_payload_client_hello[index_for_payload_handshake + 5] &= HEX_FF;
    index_for_payload_handshake = index_for_payload_handshake + 6;
  }
  else if(index_client_msg == ZERO)
  {
    size_temp = vector_ecdh.size() + 6;
    vector_handshake_payload_client_hello[index_for_payload_handshake + TWO] = (size_temp >> (ONE * EIGHT));
    vector_handshake_payload_client_hello[index_for_payload_handshake + TWO] &= HEX_FF;
    
    vector_handshake_payload_client_hello[index_for_payload_handshake + 3] = (size_temp >> (ZERO * EIGHT)); 
    vector_handshake_payload_client_hello[index_for_payload_handshake + 3] &= HEX_FF;
    size_temp = size_temp - TWO;
    
    vector_handshake_payload_client_hello[index_for_payload_handshake + 4] = (size_temp >> (ONE * EIGHT)); 
    vector_handshake_payload_client_hello[index_for_payload_handshake + 4] &= HEX_FF;
    
    vector_handshake_payload_client_hello[index_for_payload_handshake + 5] = (size_temp >> (ZERO *EIGHT));
    vector_handshake_payload_client_hello[index_for_payload_handshake + 5] &= HEX_FF;
    index_for_payload_handshake = index_for_payload_handshake + 6;
  }
  else if(index_client_msg == 4)
  {
    size_of_pre_key = extension_type_pre_shared_key.data.size() + 3 + TWO + 32;
    vector_handshake_payload_client_hello[index_for_payload_handshake + 2] = (size_of_pre_key >> (ONE * EIGHT));
    vector_handshake_payload_client_hello[index_for_payload_handshake + 2] &= HEX_FF;
    
    vector_handshake_payload_client_hello[index_for_payload_handshake + 3] = (size_of_pre_key >> (ZERO * EIGHT));   
    vector_handshake_payload_client_hello[index_for_payload_handshake + 3] &= HEX_FF;

    vector_handshake_payload_client_hello[index_for_payload_handshake + 4] = (extension_type_pre_shared_key.data.size() >> (ONE * EIGHT)); 
    vector_handshake_payload_client_hello[index_for_payload_handshake + 4] &= HEX_FF;
    
    vector_handshake_payload_client_hello[index_for_payload_handshake + 5] = (extension_type_pre_shared_key.data.size() >> (ZERO * EIGHT));
    vector_handshake_payload_client_hello[index_for_payload_handshake + 5] &= HEX_FF;
    index_for_payload_handshake = index_for_payload_handshake + 6;
  }
  
  std::memcpy(&vector_handshake_payload_client_hello[index_for_payload_handshake], 
              &temp_extension.data[0], 
              temp_extension.data.size());

  index_for_payload_handshake = index_for_payload_handshake + temp_extension.data.size();
  index_client_msg++;
}

client_msg_len = vector_handshake_payload_client_hello.size()+32+3;
handshake_message_header_.length[0] = (client_msg_len >> (TWO * EIGHT));
handshake_message_header_.length[0] &= HEX_FF;
handshake_message_header_.length[1] = (client_msg_len >> (ONE * EIGHT));
handshake_message_header_.length[1] &= HEX_FF;
handshake_message_header_.length[2] = (client_msg_len >> (ZERO * EIGHT));
handshake_message_header_.length[2] &= HEX_FF;


temp_data_from_handshake.resize(vector_handshake_payload_client_hello.size() + sizeof(handshake_message_header_));

memcpy(&temp_data_from_handshake[ZERO], 
      &handshake_message_header_, 
      sizeof(handshake_message_header_));
memcpy(&temp_data_from_handshake[sizeof(handshake_message_header_)], 
       &vector_handshake_payload_client_hello[0], 
       vector_handshake_payload_client_hello.size());


for(std::string id : psk_identities_)
{

  sha2 hash_fun_client;
  hmac_sha2::digest_storage storage_digest;
  layer_.compute_early_secrets(psks_.find(id)->second,{});
  hmac_sha2 hmac(&layer_.key_for_binding[0], layer_.key_for_binding.size());
  
  
  hash_fun_client.update(&temp_data_from_handshake[ZERO], 
                          temp_data_from_handshake.size());
  storage_digest = hash_fun_client.digest();

  hmac.update(storage_digest.data(), 
              storage_digest.size());

  vector_handshake_payload_client_hello.push_back(ZERO);
  vector_handshake_payload_client_hello.push_back(33);
  vector_handshake_payload_client_hello.push_back(32);

  for(uint8_t temp_vec : hmac.digest())  
    vector_handshake_payload_client_hello.push_back(temp_vec);
  
}


vector_data_handshake.resize(vector_handshake_payload_client_hello.size() + 
                            sizeof(handshake_message_header_));


memcpy(&vector_data_handshake[ZERO], 
       &handshake_message_header_, 
       sizeof(handshake_message_header_));


memcpy(&vector_data_handshake[sizeof(handshake_message_header_)], 
       &vector_handshake_payload_client_hello[ZERO], 
       vector_handshake_payload_client_hello.size());

layer_.write(TLS_HANDSHAKE, vector_data_handshake);
vector_of_client_msg.insert(vector_of_client_msg.end(), vector_data_handshake.begin(), vector_data_handshake.end());

}
 size_t get_lengt(handshake_message_header handshake_message_from_header)
{
  size_t return_value; 
  return_value = (size_t) (handshake_message_from_header.length[ZERO] << 16) + 
        (size_t) (handshake_message_from_header.length[ONE] << EIGHT) + 
        (size_t)handshake_message_from_header.length[TWO];
  return return_value; 
}
uint16_t calculate_the_last(std::vector<uint8_t> vector_payload_server, size_t value)
{
  uint16_t return_value; 
  return_value = (uint16_t) (vector_payload_server[value] << EIGHT) + (uint16_t) vector_payload_server[value + ONE];
                  
  return return_value; 
}
void tls_handshake_client::set_layer(cipher_suite cipher_suite_to_select, std::vector<uint8_t> vector_edch_data_share)
{
  layer_.set_cipher_suite(cipher_suite_to_select);
  layer_.compute_handshake_traffic_keys(vector_edch_data_share, vector_of_client_msg);
  layer_.update_read_key();
  layer_.update_write_key();
}
  

alert_location tls_handshake_client::read_server_hello()
{
  /// \todo Read and handle ServerHello message
  handshake_message_header handshake_message_from_header; 
  size_t size_of_header_msg; 
  size_t size_of_msg_len; 
  std::vector<uint8_t> vector_server_header;
  std::vector<uint8_t> vector_payload_server; 
  std::vector<uint8_t> vector_data_for_handshake;
  std::vector<uint8_t> vector_edch_data_share;
  alert_location alert_msg; 
  HandshakePayload handshake_payload; 
  cipher_suite cipher_suite_to_select; 
  uint16_t size_of_key; 
  size_t temp; 
  uint16_t index_at_ide;
  
  
  Extension extension_key_share;
  Extension extension_version_to_select;
  Extension extension_shared_key_pre;
  KeyShareEntry share_key_entry;

  size_of_header_msg = sizeof(handshake_message_from_header);
  //get allert msg from read
  alert_msg = layer_.read(TLS_HANDSHAKE, vector_server_header, size_of_header_msg); 
  //check if alert
  if(!alert_msg)
  {

    return alert_msg; 
  }

  //set header from server
  handshake_message_from_header.msg_type = handshake_types(vector_server_header[0]);
  for(int i = 0; i < 3; i++)
  {
    handshake_message_from_header.length[i] = vector_server_header[i + ONE];
  }

  //check the type of msg
  if(handshake_message_from_header.msg_type != handshake_types::SERVER_HELLO)
  {

    return{local,unexpected_message};
  }

  size_of_msg_len = get_lengt(handshake_message_from_header);
  //check alert for payload
  alert_msg = layer_.read(TLS_HANDSHAKE, vector_payload_server, size_of_msg_len);
  if(!alert_msg)
  {

    return alert_msg; 
  }

  if (vector_payload_server[ONE] != THREE || vector_payload_server[ZERO] != THREE)
  {

    return {local, protocol_version};
  }
  std::memcpy(&handshake_payload.random.random_bytes,
              &vector_payload_server[TWO], 32);


  handshake_payload.legacy_session_id.push_back(vector_payload_server[34]);
  if (handshake_payload.legacy_session_id[ZERO] != ZERO)
  {

    return {local, illegal_parameter}; 
  }

  cipher_suite_to_select.type[ZERO] = vector_payload_server[35];
  cipher_suite_to_select.type[ONE] = vector_payload_server[36];

  //push cipher suite to handshake payload
  handshake_payload.cipher_suites.push_back(cipher_suite_to_select);

  //check the selected suite
  if (cipher_suite_to_select != TLS_ASCON_128_SHA256)
  {
    if (cipher_suite_to_select != TLS_AES_128_GCM_SHA256)
    {
        if (cipher_suite_to_select != TLS_ISAP_128_SHA256) {
            return {local, handshake_failure};
        }
    }
  }

  //push to handshake payload
  handshake_payload.legacy_compression_methods.push_back(vector_payload_server[37]);

  if (handshake_payload.legacy_compression_methods[ZERO] != ZERO)
  {

      return {local, illegal_parameter};
  }

                                                            
  extension_key_share.type = ExtensionType(calculate_the_last(vector_payload_server, 40));
  if  (extension_key_share.type != ExtensionType::KEY_SHARE)
  {

    return {local, illegal_parameter};
  }

  share_key_entry.group = NamedGroup(calculate_the_last(vector_payload_server, 44));
  if(NamedGroup::SECP_256_R1 != share_key_entry.group)
  {

    return {local, illegal_parameter}; 
  }
    

  size_of_key = calculate_the_last(vector_payload_server, 46);
  temp = size_of_key + 48; 

  extension_key_share.data.resize(size_of_key);
  memcpy(&extension_key_share.data[ZERO], 
        &vector_payload_server[48], 
        size_of_key);

  //STart here 
                                                  
  extension_version_to_select.type = ExtensionType(calculate_the_last(vector_payload_server, temp));
  if(ExtensionType::SUPPORTED_VERSIONS != extension_version_to_select.type )
  {

    return {local, illegal_parameter};
  }


  if(vector_payload_server[TWO + temp] != ZERO || vector_payload_server[THREE + temp] != TWO)
  {

    return {local, illegal_parameter};
  }
    


  if(vector_payload_server[FOUR + temp] != TLSv1_3.major || vector_payload_server[5 + temp] != TLSv1_3.minor)
  {

    return {local, illegal_parameter};
  }
    

  //incement the indey temp
  temp = temp + 6;
  
  
  extension_shared_key_pre.type = ExtensionType(calculate_the_last(vector_payload_server, temp));
  index_at_ide = calculate_the_last(vector_payload_server, temp + FOUR);
  if(ExtensionType::PRE_SHARED_KEY !=extension_shared_key_pre.type)
  {

    return {local, illegal_parameter};
  }
  

  if(vector_payload_server[TWO + temp] != ZERO || vector_payload_server[THREE + temp] != 2)
  {

    return {local, illegal_parameter};
  }
    

  temp = temp + 6;

  if (psks_.find(psk_identities_[index_at_ide]) == psks_.end())
  {

    return {local, unknown_psk_identity};
  }
    
  size_t size_of_payload = vector_payload_server.size();
  size_t size_of_header_vector = vector_server_header.size();
   
  vector_data_for_handshake.resize(size_of_payload + size_of_header_vector);

  memcpy(&vector_data_for_handshake[ZERO], 
          &vector_server_header[ZERO], 
          size_of_header_vector);

  memcpy(&vector_data_for_handshake[size_of_header_vector], 
         &vector_payload_server[ZERO], 
         size_of_payload);

  vector_of_client_msg.insert(vector_of_client_msg.end(), 
                          vector_data_for_handshake.begin(), 
                          vector_data_for_handshake.end());

  vector_edch_data_share = ecdh_.get_shared_secret(extension_key_share.data);

  set_layer(cipher_suite_to_select, vector_edch_data_share);
  return {local, ok};
   //return {local, internal_error};
}
 


alert_location tls_handshake_client::read_finished()
{
  /// \todo Read and handle Finished message
  //return {local, internal_error};
  size_t size_of_header_msg;
	sha2 sha2_hash_client;
	handshake_message_header handshake_msg_header;
	std::vector<uint8_t> vector_key_done;
	std::vector<uint8_t> vector_hash_to_server;
	std::vector<uint8_t> vector_server_msg;
	std::vector<uint8_t> vector_client_hash;
	
	size_t size_of_msg_from_client;	
  alert_location alert_msg;

	
	alert_msg = layer_.read(TLS_HANDSHAKE, vector_server_msg, hmac_sha2::digest_size + FOUR);

    if(!alert_msg)
	{
		
		return alert_msg;
	}
	


	 handshake_msg_header.msg_type = handshake_types(vector_server_msg[ZERO]);
	 for(int i = 0; i < 3; i++)
	 {
		handshake_msg_header.length[i] = vector_server_msg[i + ONE];
	 }
	  

	  if ( handshake_types::FINISHED != handshake_msg_header.msg_type )
	  {
		  
		return {local, unexpected_message};
	  }
	  
		
//provjeri moze li moja funkcija 
  size_of_header_msg = get_lengt(handshake_msg_header);
    
  
  vector_hash_to_server.resize(size_of_header_msg);
  std::memcpy(&vector_hash_to_server[ZERO], &vector_server_msg[FOUR], size_of_header_msg);

  size_of_msg_from_client = vector_of_client_msg.size();
  sha2_hash_client.update(&vector_of_client_msg[ZERO], size_of_msg_from_client);
  hmac_sha2::digest_storage dig = sha2_hash_client.digest();

  vector_key_done  = layer_.get_finished_key(connection_end::SERVER);
  //todo: check hmac
  hmac_sha2 hmac(&vector_key_done[0], vector_key_done.size());  
  hmac.update(dig.data(), dig.size());
  
  
  sha2::digest_storage digest_store_hash = hmac.digest();
  size_t size_hmac = digest_store_hash.size();
	  
	vector_client_hash.resize(digest_store_hash.size());
	std::memcpy(&vector_client_hash[ZERO], &digest_store_hash[ZERO], size_hmac);

	if(vector_client_hash != vector_hash_to_server)
	{
		
		return {local, decrypt_error};
	}
		

	vector_of_client_msg.insert(vector_of_client_msg.end(), 
                              vector_server_msg.begin(), 
                              vector_server_msg.end());

	return {local, ok};
}
void tls_handshake_client::push_to_layer(std::vector<uint8_t> vector_to_server)
{
  layer_.write(TLS_HANDSHAKE, vector_to_server);
  layer_.compute_application_traffic_keys(vector_of_client_msg);
  layer_.update_read_key();
  layer_.update_write_key();
  vector_of_client_msg.insert(vector_of_client_msg.end(), vector_to_server.begin(), vector_to_server.end()); 

}
void tls_handshake_client::send_finished()
{
/// \todo Send Finished message.
  
  sha2 sha2_hash_to_client;
  size_t size_of_client_msg = vector_of_client_msg.size();
  handshake_message_header handshake_msg_header;
  hmac_sha2::digest_storage hmac_sha2_digest_store;
  std::vector<uint8_t> vector_signal_end;
  //declare a type
  handshake_msg_header.msg_type = handshake_types::FINISHED;
  for (int i = 0; i < 2; i++)
  {
		handshake_msg_header.length[i] = ZERO;
  }
  handshake_msg_header.length[2] = hmac_sha2::digest_size;

  std::vector<uint8_t> vector_to_server;
  vector_to_server.push_back(handshake_types::FINISHED);
  for (int i = 0; i < 3; i++)
  {
	  vector_to_server.push_back(handshake_msg_header.length[i]);
  }
  
  
  sha2_hash_to_client.update(&vector_of_client_msg[ZERO],
							size_of_client_msg);
  hmac_sha2_digest_store = sha2_hash_to_client.digest();
	
  sha2::digest_storage digest_store_hash;	
  vector_signal_end = layer_.get_finished_key(connection_end::CLIENT);
  hmac_sha2 hmac(&vector_signal_end[ZERO], vector_signal_end.size());  
  hmac.update(hmac_sha2_digest_store.data(), 
			  hmac_sha2_digest_store.size());

  digest_store_hash  = hmac.digest();

  for(auto it : digest_store_hash)
  {
		vector_to_server.push_back(it);
  }

  push_to_layer(vector_to_server);

}

