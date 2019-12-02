#include "tls-handshake-server.h"
#define THREE 3
#define EIGHT 8

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

alert_location tls_handshake_server::setCurrentAlert(alert_location currentAlert_)
{
    currentAlert = currentAlert_;
}

alert_location tls_handshake_server::checkError()
{
    if(currentAlert == false)
        return currentAlert;
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

  //get client_header
  handshake_message_header clientHeaderHandshake = returnClientHeader();
  if(handshake_types::CLIENT_HELLO != clientHeaderHandshake.msg_type)
  {
      return {local, unexpected_message};
  }

  size_t payload_length_param_1 = clientHeaderHandshake.length[0] << (EIGHT + EIGHT);
  size_t payload_length_param_2 = clientHeaderHandshake.length[1] << EIGHT;
  size_t payload_length = (size_t)(payload_length_param_1 + payload_length_param_2) + (size_t)(clientHeaderHandshake.length[2]);

  std::vector<uint8_t > clientPayload = getClientPayload(payload_length);
  if(clientPayload.at(0) != THREE)//probaj umjesto .at [] TODO
      return {local, protocol_version};
  else if(clientPayload.at(1) != THREE)
      return {local, protocol_version};

  HandshakePayload clientHello_random;
  std::memcpy(&clientHello_random.random.random_bytes, &clientPayload[2], num_32);
  clientHello_random.legacy_session_id.push_back(clientPayload.at(num_32 + 2));

  if(clientHello_random.legacy_session_id.at(0) != 0)
      return {local, illegal_parameter};

  //TODO probaj dole staviti gdje se koritstin
    std::vector<cipher_suite> cipher_suites_vector;
    uint16_t length_of_cipher_suites_param1 = clientPayload.at(35) << EIGHT;
    size_t length_of_cipher_suites = (uint16_t)(clientPayload.at(36)) + (uint16_t)(length_of_cipher_suites_param1);
    auto i = 0;
    while(i < length_of_cipher_suites)
    {
        cipher_suite cipher_suite_selected;
        cipher_suite_selected.type[1] = clientPayload.at(i  + 38);
        cipher_suite_selected.type[0] = clientPayload.at(i  + 37);

        if(cipher_suite_selected != TLS_ASCON_128_SHA256 && cipher_suite_selected != TLS_AES_128_GCM_SHA256)
            return {local, handshake_failure};

        cipher_suites_vector.push_back(cipher_suite_selected);
        i++;
    }

  Extension shareKey = getKeyShare(clientPayload);
  if(shareKey.type != ExtensionType::KEY_SHARE)
      return {local, illegal_parameter};

  //rest??TODO
  KeyShareEntry shareKeyEntry = getKeyShareEntry(clientPayload);
  if(shareKeyEntry.group != NamedGroup::SECP_256_R1)
      return {local, illegal_parameter};

  uint16_t  size_of_key_param1 = clientPayload.at(53) << EIGHT;
  uint16_t size_of_key = (uint16_t)(size_of_key_param1) + (uint16_t)(clientPayload.at(54));

  setKeyForClient(clientPayload, size_of_key);//da li treba isto ovo za shareKeyEntry?? TODO

  setCurrentPosition(size_of_key + 55);
  Extension selectedVersion = getExtension(clientPayload);//da li ovo moze za sve Extension TODO

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

  setCurrentPosition(currentPosition + 2);

  size_t  id_selected = getSizeOfIdentites(clientPayload, 0);//TODO probaj bez ovoga samo obrisi i dole stavi = 0, jer ne koristi ga nikako do dole

  setCurrentPosition(currentPosition + 2);
  std::vector<std::string> identities_vector;
  size_t sizeOfIdentities = getSizeOfIdentites(clientPayload, 0);
  for(auto i = 0; i < sizeOfIdentities; i++)
  {
      if(i == 0 || i == 1)
          continue;

      size_t id_size = getSizeOfIdentites(clientPayload, i);
      size_t iterator = 0;
      std::string id = NULL;//TODO provjeri
      while(iterator < id_size)
      {
          char letter_char = (char) clientPayload.at(currentPosition + 2 + iterator + i);
          id.push_back(letter_char);
          iterator++;
      }
      identities_vector.push_back(id);
      i = id_size + i + 4;
  }
  id_selected = 0;//TODO probaj moze li samo 0 umjesto njega

  uint16_t size_of_bidner_param1 = clientPayload[currentPosition + sizeOfIdentities + 2] >> 8;
  size_of_bidner_param1 = size_of_bidner_param1 & 0xFF;

  uint16_t size_of_bidner_param2 = clientPayload[currentPosition + sizeOfIdentities + 3] >> 8;
  size_of_bidner_param2 = size_of_bidner_param2 & 0xFF;
  size_t size_of_bidner = (uint16_t)(size_of_bidner_param1) +  (uint16_t)(size_of_bidner_param2);

  std::vector<uint8_t > bidners_vector;
  bidners_vector.resize(size_of_bidner);//TODO provjeri jel moze
  std::memcpy(&bidners_vector[0], &clientPayload[4 + sizeOfIdentities + currentPosition], size_of_bidner);

  if(psks_.end() ==  psks_.find(identities_vector.at(id_selected)))
    return {local, internal_error};

  std::vector<uint8_t> psk_find = psks_.find(identities_vector.at(id_selected))->second;
  layer_.compute_early_secrets(psk_find, {});

  selected_cipher_suite = cipher_suites_vector.at(0);

  //provjeri jel dobar clientHeader TODO
  std::vector<uint8_t > data_handshake_server(clientHeader.size() + clientPayload.size());//TODO da li treba resize
  memcpy(&data_handshake_server[0], &clientHeader[0], clientHeader.size());
  memcpy(&data_handshake_server[clientHeader.size()], &clientPayload[0], clientPayload.size());

  serverMessagesVector.insert(serverMessagesVector.end(), data_handshake_server.begin(), data_handshake_server.end());
  return {local, ok};
}

void tls_handshake_server::send_server_hello()
{
/// \todo write the ServerHello message to the record layer
/// If have_fixed_randomness_ is false, generate random data.
/// If it is true, use fixed_randomness_ as random data.

}

void tls_handshake_server::send_finished()
{
/// \todo write the Finished message to the record layer
}

alert_location tls_handshake_server::read_finished()
{
  /// \todo read the Finished message from the record layer and handle it
  return {local, internal_error};
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

