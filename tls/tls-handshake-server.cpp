#include "tls-handshake-server.h"
#define THREE 3

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

handshake_message_header tls_handshake_server::returnClientHeader()
{
    handshake_message_header client_header_reader;
    std::vector<uint8_t> clientHeader;
    setCurrentAlert(layer_.read(TLS_HANDSHAKE, clientHeader, sizeof(client_header_reader)));
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
    uint16_t extension_param_1 = clientPayload.at(45) << 8;
    Extension shareKey;
    shareKey.type = ExtensionType ((uint16_t)(extension_param_1) + (uint16_t)(clientPayload.at(46)));
    return shareKey;
}

KeyShareEntry tls_handshake_server::getKeyShareEntry(std::vector<uint8_t> clientPayload)
{
    uint16_t shareKeyEntry_param1 = clientPayload.at(51) << 8;
    KeyShareEntry shareKeyEntry;
    shareKeyEntry.group = NamedGroup ((uint16_t)(shareKeyEntry_param1) + (uint16_t)(clientPayload.at(52)));
    return shareKeyEntry;
}

alert_location tls_handshake_server::read_client_hello()
{
  /// \todo read the ClientHello message from the record layer and handle it

  //get client_header
  handshake_message_header clientHeader = returnClientHeader();
  if(handshake_types::CLIENT_HELLO != clientHeader.msg_type)
  {
      return {local, unexpected_message};
  }

  size_t payload_length_param_1 = clientHeader.length[0] << 16;
  size_t payload_length_param_2 = clientHeader.length[1] << 8;
  size_t payload_length = (size_t)(payload_length_param_1 + payload_length_param_2) + (size_t)(clientHeader.length[2]);

  std::vector<uint8_t > clientPayload = getClientPayload(payload_length);
  if(clientPayload.at(0) != THREE)//probaj umjesto .at []
      return {local, protocol_version};
  else if(clientPayload.at(1) != THREE)
      return {local, protocol_version};

  HandshakePayload clientHello_random;
  std::memcpy(&clientHello_random.random.random_bytes, &clientPayload[2], num_32);
  clientHello_random.legacy_session_id.push_back(clientPayload.at(num_32 + 2));

  if(clientHello_random.legacy_session_id.at(0) != 0)
      return {local, illegal_parameter};

  Extension shareKey = getKeyShare(clientPayload);
  if(shareKey.type != ExtensionType::KEY_SHARE)
      return {local, illegal_parameter};

  //rest??
  KeyShareEntry shareKeyEntry = getKeyShareEntry(clientPayload);
  if(shareKeyEntry.group != NamedGroup::SECP_256_R1)
      return {local, illegal_parameter};



  return {local, internal_error};
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

