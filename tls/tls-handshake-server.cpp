#include "tls-handshake-server.h"
#include "random.h"
#include "hmac-sha2.h"

#define THREE 3
#define EIGHT 8
#define SERVER_LEGACY 0x0303;
#define SHIFT_NUMBER 0xFF;

tls_handshake_server::tls_handshake_server(tls_record_layer& layer, const psk_map& psks)
  : layer_(layer), psks_(psks), ecdh_(SECP256R1), fixed_randomness_({{0}}),
    have_fixed_randomness_(false)
{
  ecdh_.generate_key_pair();
}

tls_handshake_server::tls_handshake_server(tls_record_layer& layer, const psk_map& psks,
                                           const random_struct& fixed_randomness,
                                           const gfp_t& ecdh_private)
  : layer_(layer), psks_(psks), ecdh_(SECP256R1), fixed_randomness_(fixed_randomness),
    have_fixed_randomness_(true)
{
  ecdh_.set_private_key(ecdh_private);
}

alert_location tls_handshake_server::checkError()
{
    if(currentAlert == false)
        return currentAlert;

    return {};
}

void tls_handshake_server::setCurrentAlert(alert_location currentAlert_)
{
    currentAlert = currentAlert_;
}

std::vector<uint8_t> tls_handshake_server::getKeyForClient()
{
    return keyForClient;
}

void tls_handshake_server::setKeyForClient(std::vector<uint8_t> keyForClient_)
{
    keyForClient = keyForClient_;
}

void tls_handshake_server::setClientHeader(std::vector<uint8_t> clientHeader)
{
    clientHeader = clientHeader;
}

handshake_message_header tls_handshake_server::returnClientHeader()
{
    handshake_message_header client_header_reader;
    std::vector<uint8_t> clientHeader;

    setCurrentAlert(layer_.read(TLS_HANDSHAKE, clientHeader, sizeof(client_header_reader)));
    setClientHeader(clientHeader);
    checkError();


    client_header_reader.length[0] = clientHeader[1];
    client_header_reader.length[1] = clientHeader[2];
    client_header_reader.length[2] = clientHeader[3];
    client_header_reader.msg_type = handshake_types(clientHeader[0]);//
    return client_header_reader;

}

std::vector<uint8_t> tls_handshake_server::getClientPayload(size_t payload_length_)
{
    std::vector<uint8_t> clientPayload;
    setCurrentAlert(layer_.read(TLS_HANDSHAKE, clientPayload, payload_length_));
    checkError();

    return clientPayload;
}

Extension tls_handshake_server::getKeyShare(std::vector<uint8_t> clientPayload)
{
    uint16_t extension_param_1 = clientPayload.at(45) << EIGHT;
    Extension shareKey;
    shareKey.type = ExtensionType ((uint16_t)(extension_param_1) + (uint16_t)(clientPayload.at(46)));
    return shareKey;
}

KeyShareEntry tls_handshake_server::getKeyShareEntry(std::vector<uint8_t> clientPayload)
{
    uint16_t shareKeyEntry_param1 = clientPayload.at(51) << EIGHT;
    KeyShareEntry shareKeyEntry;
    shareKeyEntry.group = NamedGroup ((uint16_t)(shareKeyEntry_param1) + (uint16_t)(clientPayload.at(52)));
    return shareKeyEntry;
}
void tls_handshake_server::setKeyForClient(std::vector<uint8_t> clientPayload, uint16_t size_of_key)
{
    keyForClient.resize(size_of_key);
    memcpy(&keyForClient[0],&clientPayload[55],size_of_key);
}

Extension tls_handshake_server::getExtension(std::vector<uint8_t> clientPayload)
{
    Extension selectedVersion;
    uint16_t selectedVersion_param1 = clientPayload.at(currentPosition) << EIGHT;
    selectedVersion.type = ExtensionType ((uint16_t)(selectedVersion_param1) + (uint16_t)(clientPayload.at(currentPosition + 1)));
    return  selectedVersion;
}

void tls_handshake_server::setCurrentPosition(size_t size)
{
    currentPosition = size;
}

size_t tls_handshake_server::getSizeOfIdentites(std::vector<uint8_t> clientPayload, size_t iterator)
{

    uint16_t sizeOfIdentities = clientPayload.at(currentPosition + iterator) << EIGHT;
    return (uint16_t)(sizeOfIdentities) + (uint16_t)(clientPayload.at(currentPosition + 1 + iterator));

}


alert_location tls_handshake_server::read_client_hello()
{
    /// \todo read the ClientHello message from the record layer and handle it
    std::vector<cipher_suite> cipher_suites_vector;
    handshake_message_header clientHeaderHandshake;

    std::vector<uint8_t> clientHeader;
    setCurrentAlert(layer_.read(TLS_HANDSHAKE, clientHeader, sizeof(clientHeaderHandshake)));
    setClientHeader(clientHeader);
    //checkError();
    if(currentAlert == false)
        return currentAlert;

    clientHeaderHandshake.msg_type = handshake_types(clientHeader[0]);
    clientHeaderHandshake.length[0] = clientHeader[1];
    clientHeaderHandshake.length[1] = clientHeader[2];
    clientHeaderHandshake.length[2] = clientHeader[3];

    if(clientHeaderHandshake.msg_type != handshake_types::CLIENT_HELLO)
        return {local, unexpected_message};

    size_t payload_length_param_1 = clientHeaderHandshake.length[0] << (EIGHT + EIGHT);
    size_t payload_length_param_2 = clientHeaderHandshake.length[1] << EIGHT;
    size_t payload_length = (size_t)(payload_length_param_1 + payload_length_param_2) + (size_t)(clientHeaderHandshake.length[2]);

    std::vector<uint8_t> clientPayload = getClientPayload(payload_length);;

    if(clientPayload.at(0) != THREE)//probaj umjesto .at [] TODO
        return {local, protocol_version};
    else if(clientPayload.at(1) != THREE)
        return {local, protocol_version};

    HandshakePayload clientHello_random;
    std::memcpy(&clientHello_random.random.random_bytes, &clientPayload[2], num_32);
    clientHello_random.legacy_session_id.push_back(clientPayload.at(num_32 + 2));

    if(clientHello_random.legacy_session_id.at(0) != 0)
        return {local, illegal_parameter};



    uint16_t length_of_cipher_suites_param1 = clientPayload.at(35) << EIGHT;
    size_t length_of_cipher_suites = (uint16_t)(clientPayload.at(36)) + (uint16_t)(length_of_cipher_suites_param1);
    size_t i = 0;
    while(i < length_of_cipher_suites)
    {
        cipher_suite cipher_suite_selected;
        cipher_suite_selected.type[1] = clientPayload.at(i  + 38);
        cipher_suite_selected.type[0] = clientPayload.at(i  + 37);

        if(cipher_suite_selected != TLS_ASCON_128_SHA256 && cipher_suite_selected != TLS_AES_128_GCM_SHA256 && cipher_suite_selected != TLS_ISAP_128_SHA256)
            return {local, handshake_failure};

        cipher_suites_vector.push_back(cipher_suite_selected);
        i++;
        i++;
    }
    Extension shareKey = getKeyShare(clientPayload);;

    if(shareKey.type != ExtensionType::KEY_SHARE)
        return {local, illegal_parameter};


    KeyShareEntry shareKeyEntry = getKeyShareEntry(clientPayload);;

    if(shareKeyEntry.group != NamedGroup::SECP_256_R1)
        return {local, illegal_parameter};


    uint16_t  size_of_key_param1 = clientPayload.at(53) << EIGHT;
    uint16_t size_of_key = (uint16_t)(size_of_key_param1) + (uint16_t)(clientPayload.at(54));

    keyForClient.resize(size_of_key);
    memcpy(&keyForClient[0], &clientPayload[55], size_of_key);

    setCurrentPosition(size_of_key + 55);

    Extension selectedVersion = getExtension(clientPayload);

    if(selectedVersion.type != ExtensionType::SUPPORTED_VERSIONS || clientPayload.at(currentPosition + 2) != 0 || clientPayload.at(currentPosition + 3) != 3 || clientPayload.at(currentPosition + 4) != 2
       || clientPayload.at(currentPosition + 5) != TLSv1_3_MAJOR || clientPayload.at(currentPosition + 6) != TLSv1_3_MINOR)
        return {local, illegal_parameter};

    setCurrentPosition(currentPosition + 7);

    Extension pskEx = getExtension(clientPayload);

    if(pskEx.type != ExtensionType::PSK_KEY_EXCHANGE_MODES || clientPayload.at(currentPosition + 2) != 0 || clientPayload.at(currentPosition + 3) != 2
       || clientPayload.at(currentPosition + 4) != 1 || clientPayload.at(currentPosition + 5) != PskKeyExchangeMode::PSK_DHE_KE)
        return {local, illegal_parameter};

    setCurrentPosition(currentPosition + 6);

    Extension supportedGroups_ = getExtension(clientPayload);

    if(supportedGroups_.type != ExtensionType::SUPPORTED_GROUPS || clientPayload.at(currentPosition + 2) != 0 || clientPayload.at(currentPosition + 3) != 4
       || clientPayload.at(currentPosition + 4) != 0 || clientPayload.at(currentPosition + 5) != 2
       || clientPayload.at(currentPosition + 6) != 0x0 || clientPayload.at(currentPosition + 7) != 0x17) //moze li 0x00 TODO
        return {local, illegal_parameter};

    setCurrentPosition(currentPosition + 8);

    Extension preSharedKey = getExtension(clientPayload);

    if(preSharedKey.type != ExtensionType::PRE_SHARED_KEY)
        return {local, illegal_parameter};

    setCurrentPosition(currentPosition + 4);

    size_t sizeOfIdentities = (uint16_t) (clientPayload.at(currentPosition) << 8) + (uint16_t) clientPayload.at(currentPosition + 1);

    std::vector<std::string> identities_vector;
    for(size_t i = 0; i < sizeOfIdentities; i++)
    {
        if(i == 0 || i == 1)
            continue;

        size_t id_size = getSizeOfIdentites(clientPayload, i);
        std::string id;
        size_t iterator = 0;
        while(iterator < id_size)
        {
            char letter_char = (char) clientPayload.at(currentPosition + 2 + iterator + i);
            id.push_back(letter_char);
            iterator++;
        }
        identities_vector.push_back(id);
        i = id_size + i + 4;
    }

    uint16_t size_of_bidner_param1 = clientPayload[currentPosition + sizeOfIdentities + 2] >> 8;
    size_of_bidner_param1 = size_of_bidner_param1 & 0xFF;

    uint16_t size_of_bidner_param2 = clientPayload[currentPosition + sizeOfIdentities + 3];
    size_of_bidner_param2 = size_of_bidner_param2 & 0xFF;
    size_t size_of_bidner = (uint16_t)(size_of_bidner_param1) +  (uint16_t)(size_of_bidner_param2);


    std::vector<uint8_t > bidners_vector;
    bidners_vector.resize(size_of_bidner);//TODO provjeri jel moze
    std::memcpy(&bidners_vector[0], &clientPayload[4 + sizeOfIdentities + currentPosition], size_of_bidner);


    if(psks_.end() ==  psks_.find(identities_vector.at(ident_selected)))
        return {local, internal_error};

    std::vector<uint8_t> psk_find = psks_.find(identities_vector.at(ident_selected))->second;//TODO moze li nula ovdje se koristiti
    layer_.compute_early_secrets(psk_find, {});
    selected_cipher_suite = cipher_suites_vector.at(0);

    std::vector<uint8_t > data_handshake_server(clientHeader.size() + clientPayload.size());

    memcpy(&data_handshake_server[0], &clientHeader[0], clientHeader.size());
    memcpy(&data_handshake_server[clientHeader.size()], &clientPayload[0], clientPayload.size());

    serverMessagesVector.insert(serverMessagesVector.end(), data_handshake_server.begin(), data_handshake_server.end());

    return {local, ok};
}

void tls_handshake_server::setSereverHelloMessageSize(size_t size)
{
    serverHelloMessageSize = serverHelloMessageSize + size;
}

void tls_handshake_server::setExtenesionSize(size_t size)
{
    extensionSize = extensionSize + size;
}

void tls_handshake_server::send_server_hello()
{

    size_t position = 20 + 20;
    serverHelloMessageSize = 0;


    handshake_message_header handshakeMessageHeader;
    handshakeMessageHeader.msg_type = handshake_types::SERVER_HELLO;

    HandshakePayload serverHelloMessage;
    serverHelloMessage.legacy_version = 0x0303;


    if(!have_fixed_randomness_)
    {
        get_random_data(reinterpret_cast<uint8_t *>(serverHelloMessage.random.random_bytes), num_32);
    }
    else{
        std::memcpy(&serverHelloMessage.random.random_bytes, &fixed_randomness_, num_32);

    }

    serverHelloMessage.legacy_compression_methods.push_back(0);

    serverHelloMessage.legacy_session_id.push_back(0);

    serverHelloMessage.cipher_suites.push_back(selected_cipher_suite);


    Extension versionSupported;

    versionSupported.data.push_back(TLSv1_3.major);
    versionSupported.data.push_back(TLSv1_3.minor);
    versionSupported.type = ExtensionType::SUPPORTED_VERSIONS;

    Extension sharedKey;

    KeyShareEntry keyShareEntry;
    std::vector<uint8_t> data_for_ecdh = ecdh_.get_data();
    keyShareEntry.group = NamedGroup::SECP_256_R1;
    sharedKey.type = ExtensionType::KEY_SHARE;

    sharedKey.data.resize(ecdh_.get_data().size() + 2 + 2);//TODO check
    sharedKey.data[3] = (ecdh_.get_data().size()  & 0xFF);
    sharedKey.data[2] = (ecdh_.get_data().size()  >> 8 & 0xFF);
    NamedGroup sharedKeyShiftValue = keyShareEntry.group;
    sharedKey.data[1] = (sharedKeyShiftValue  & 0xFF);
    sharedKey.data[0] = (sharedKeyShiftValue  >> 8 & 0xFF);

    memcpy(&sharedKey.data[2+2], &data_for_ecdh[0], ecdh_.get_data().size());


    setExtenesionSize(sharedKey.data.size() + 10);

    Extension sharedKeyPre;
    sharedKeyPre.data.push_back((ident_selected >> 8) & 0xFF);
    sharedKeyPre.data.push_back(ident_selected & 0xFF);

    sharedKeyPre.type = ExtensionType::PRE_SHARED_KEY;
    setExtenesionSize(sharedKeyPre.data.size() + 3 + 1);

    serverHelloMessage.extensions.push_back(sharedKey);

    serverHelloMessage.extensions.push_back(versionSupported);
    serverHelloMessage.extensions.push_back(sharedKeyPre);

    setSereverHelloMessageSize(num_32  + 6);
    std::vector<uint8_t >handshakePayload;
    handshakePayload.resize(2 + extensionSize + serverHelloMessageSize);//TODO da li moze resize ovdje

    std::memcpy(&handshakePayload[0], &serverHelloMessage.legacy_version,2);
    std::memcpy(&handshakePayload[37], &serverHelloMessage.legacy_compression_methods[0],2 - 1);

    std::memcpy(&handshakePayload[35], &serverHelloMessage.cipher_suites[0],2);
    std::memcpy(&handshakePayload[34], &serverHelloMessage.legacy_session_id[0],1);

    std::memcpy(&handshakePayload[2], &serverHelloMessage.random.random_bytes,32);

    handshakePayload[39] = (uint8_t)(extensionSize & 0xFF);
    handshakePayload[38] = (uint8_t)((extensionSize >> EIGHT * 1) & 0xFF);

    size_t i = 0;
    while(i < serverHelloMessage.extensions.size())
    {
        handshakePayload[position] = ((serverHelloMessage.extensions[i].type >> EIGHT) & 0xFF);
        handshakePayload[position + 1] = ((serverHelloMessage.extensions[i].type >> 0) & 0xFF);


        if(i == 0)
        {
            handshakePayload[position + 2] = (uint16_t)(((data_for_ecdh.size() + 4) >> EIGHT) & 0xFF);
            handshakePayload[position + 3] = (uint16_t)(((data_for_ecdh.size() + 4) >> 0) & 0xFF);//TODO CHECK
            position = position + 4;
        }
        else if(i == 1)
        {
            handshakePayload[position + 2] = (uint16_t)(((serverHelloMessage.extensions[i].data.size()) >> EIGHT) & 0xFF);
            handshakePayload[position + 3] = (uint16_t)(((serverHelloMessage.extensions[i].data.size()) >> 0) & 0xFF);
            position = position + 4;
        }
        else if(i == 2)
        {
            handshakePayload[position + 2] = (uint16_t)(((sharedKeyPre.data.size()) >> EIGHT) & 0xFF);
            handshakePayload[position + 3] = (uint16_t)(((sharedKeyPre.data.size()) >> 0) & 0xFF);
            position = position + 4;
        }

        std::memcpy(&handshakePayload[position], &serverHelloMessage.extensions[i].data[0], serverHelloMessage.extensions[i].data.size());
        position = serverHelloMessage.extensions[i].data.size() + position;
        i++;
    }

    handshakeMessageHeader.length[2] = ((handshakePayload.size() >> 0) & 0xFF);
    handshakeMessageHeader.length[1] = ((handshakePayload.size() >> 8) & 0xFF);
    handshakeMessageHeader.length[0] = ((handshakePayload.size() >> 16) & 0xFF);

    size_t size_ = sizeof(handshakeMessageHeader);
    std::vector<uint8_t>data_for_client(size_ + handshakePayload.size());//TODO check

    memcpy(&data_for_client[0], &handshakeMessageHeader, size_);
    memcpy(&data_for_client[sizeof(handshakeMessageHeader)], &handshakePayload[0], handshakePayload.size());

    layer_.write(TLS_HANDSHAKE, data_for_client);
    serverMessagesVector.insert(serverMessagesVector.end(), data_for_client.begin(), data_for_client.end());
}


std::vector<uint8_t > tls_handshake_server::vectorForSending()
{
    handshake_message_header handshakeMessageHeader;
    handshakeMessageHeader.length[2] = hmac_sha2::digest_size;
    handshakeMessageHeader.length[1] = 0;
    handshakeMessageHeader.length[0] = 0;
    handshakeMessageHeader.msg_type = handshake_types ::FINISHED;

    std::vector<uint8_t > vector_for_sending;
    vector_for_sending.push_back((handshakeMessageHeader.msg_type));
    vector_for_sending.push_back((handshakeMessageHeader.length[0]));
    vector_for_sending.push_back((handshakeMessageHeader.length[1]));
    vector_for_sending.push_back((handshakeMessageHeader.length[2]));

    return vector_for_sending;
}

void tls_handshake_server::send_finished()
{
/// \todo write the Finished message to the record layer
    /*layer_.update_read_key();
    layer_.update_write_key();
    layer_.compute_handshake_traffic_keys(ecdh_.get_shared_secret(keyForClient), serverMessagesVector);
    layer_.set_cipher_suite(selected_cipher_suite);

    std::vector<uint8_t > vector_for_sending = vectorForSending();

    sha2 hash_client;

    hash_client.update(&serverMessagesVector[0], serverMessagesVector.size());


    std::vector<uint8_t> keyFinished = layer_.get_finished_key(connection_end::SERVER);
    hmac_sha2 HMAC_SHA2(&keyFinished[0], keyFinished.size());
    HMAC_SHA2.update(hash_client.digest().data(), hash_client.digest().size());

    for(size_t i = 0; i < HMAC_SHA2.digest().size(); i++)
    {
        vector_for_sending.push_back(HMAC_SHA2.digest()[i]);//TODO check
    }

    serverMessagesVector.insert(serverMessagesVector.end(), vector_for_sending.begin(), vector_for_sending.end());*/
  layer_.set_cipher_suite(selected_cipher_suite);

  layer_.compute_handshake_traffic_keys(ecdh_.get_shared_secret(keyForClient), serverMessagesVector);
  
  layer_.update_read_key();
  
  layer_.update_write_key();

  std::vector<uint8_t> vector_for_sending = vectorForSending();


  sha2 client_hash;
  client_hash.update(&serverMessagesVector[0], serverMessagesVector.size());
  

  std::vector<uint8_t> keyFinished;
  keyFinished = layer_.get_finished_key(connection_end::SERVER);
  hmac_sha2 HMAC_SHA2(&keyFinished[0], keyFinished.size());  
  hmac_sha2::digest_storage digest_storage = client_hash.digest();
  HMAC_SHA2.update(digest_storage.data(), client_hash.digest().size());

  auto hmac_digest = HMAC_SHA2.digest();

  for (size_t i = 0; i < HMAC_SHA2.digest().size(); i++)
  {
    vector_for_sending.push_back(hmac_digest[i]);
  }

  layer_.write(TLS_HANDSHAKE, vector_for_sending);

  serverMessagesVector.insert(serverMessagesVector.end(), vector_for_sending.begin(), vector_for_sending.end()); 
}

size_t tls_handshake_server::getLengthForClientHash(handshake_message_header handshakeMessageHeader)
{
    return (size_t)(handshakeMessageHeader.length[2] + (handshakeMessageHeader.length[1] << EIGHT) + (handshakeMessageHeader.length[0] << EIGHT * 2));
}

alert_location tls_handshake_server::read_finished()
{
  /// \todo read the Finished message from the record layer and handle it

  /*std::vector<uint8_t> finished_message_fromClient;
   alert_location alert = layer_.read(TLS_HANDSHAKE, finished_message_fromClient, 2 + 2 + hmac_sha2::digest_size);

  if(!alert)
    return alert;

  handshake_message_header handshakeMessageHeader;
  handshakeMessageHeader.msg_type = handshake_types (finished_message_fromClient[0]);
  handshakeMessageHeader.length[0] = finished_message_fromClient[1];
  handshakeMessageHeader.length[1] = finished_message_fromClient[2];
  handshakeMessageHeader.length[2] = finished_message_fromClient[3];
  
  
  if(handshake_types::FINISHED != handshakeMessageHeader.msg_type)
  {

      return {local, unexpected_message};
  }

  std::vector<uint8_t> hash_for_client;
  hash_for_client.resize(getLengthForClientHash(handshakeMessageHeader));//TODO provjeri jel treba resize
  std::memcpy(&hash_for_client[0], &finished_message_fromClient[2 +2], getLengthForClientHash(handshakeMessageHeader));
  
  sha2 hash_for_server;
  hash_for_server.update(&serverMessagesVector[0], serverMessagesVector.size());
  hmac_sha2::digest_storage dig = hash_for_server.digest();

  std::vector<uint8_t> keyFinishedVector = layer_.get_finished_key(connection_end::CLIENT);

  hmac_sha2 hmacSha2(&keyFinishedVector[0], keyFinishedVector.size());
  hmacSha2.update(dig.data(), dig.size());

  auto h1 = hmacSha2.digest();

  std::vector<uint8_t> hash;
  hash.resize(h1.size);//TODO check resize
  std::memcpy(&hash[0], &h1[0], h1.size());
  if(hash != hash_for_client)
  {

      return {local, decrypt_error};
  }
  layer_.compute_application_traffic_keys(serverMessagesVector);
  layer_.update_read_key();
  layer_.update_write_key();
  

  serverMessagesVector.insert(serverMessagesVector.end(), finished_message_fromClient.begin(), finished_message_fromClient.end());
  return {local, ok};*/

  std::vector<uint8_t> finished_message_fromClient;


  setCurrentAlert(layer_.read(TLS_HANDSHAKE, finished_message_fromClient, 2 + 2 + hmac_sha2::digest_size));
  //checkError();
    if(currentAlert == false)
        return currentAlert;

    handshake_message_header handshakeMessageHeader;
    
    
    handshakeMessageHeader.length[1] = finished_message_fromClient[2];
    handshakeMessageHeader.length[2] = finished_message_fromClient[3];
    handshakeMessageHeader.length[0] = finished_message_fromClient[1];

    handshakeMessageHeader.msg_type = handshake_types (finished_message_fromClient[0]);

  if(handshake_types::FINISHED != handshakeMessageHeader.msg_type)
  {

      return {local, unexpected_message};
  }

  std::vector<uint8_t> hash_for_client(getLengthForClientHash(handshakeMessageHeader));
  std::memcpy(&hash_for_client[0], &finished_message_fromClient[4], getLengthForClientHash(handshakeMessageHeader));
  sha2 hash_for_server;
  hash_for_server.update(&serverMessagesVector[0], serverMessagesVector.size());

 

  std::vector<uint8_t> keyFinishedVector = layer_.get_finished_key(connection_end::CLIENT);

  hmac_sha2 hmacSha2(&keyFinishedVector[0], keyFinishedVector.size());
  hmac_sha2::digest_storage digest_storage = hash_for_server.digest();
  hmacSha2.update(digest_storage.data(), digest_storage.size());
  auto hmacSha2Digest = hmacSha2.digest();

  std::vector<uint8_t> hash(hmacSha2.digest().size());
  
  std::memcpy(&hash[0], &(hmacSha2Digest)[0], hmacSha2.digest().size());
  if(hash != hash_for_client)
  {

      return {local, decrypt_error};
  }

  layer_.compute_application_traffic_keys(serverMessagesVector);

  layer_.update_read_key();


  layer_.update_write_key();

  serverMessagesVector.insert(serverMessagesVector.end(), finished_message_fromClient.begin(), finished_message_fromClient.end());
  return {local, ok};
}

alert_location tls_handshake_server::answer_handshake()
{
  // read ClientHello
  alert_location alert = read_client_hello();
  if (!alert)
    return alert;
  // send ServerHello
  send_server_hello();
  // send Finished
  send_finished();
  // read Finished
  return read_finished();
}

