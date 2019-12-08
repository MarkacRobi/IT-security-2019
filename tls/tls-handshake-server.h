#ifndef TLS_HANDSHAKE_SERVER_H
#define TLS_HANDSHAKE_SERVER_H

#include <map>
#include <vector>

#include "ecdh.h"
#include "tls-record-layer.h"
#include "tls.h"

/// Server side of the handshake
class tls_handshake_server
{
public:
  /// Instantiate new server-side handshake.
  tls_handshake_server(tls_record_layer& layer, const psk_map& psks);
  /// Instantiate new server-side handshake with fixed randomness and fixed ECDH private key.
  tls_handshake_server(tls_record_layer& layer, const psk_map& psks,
                       const random_struct& fixed_randomness, const gfp_t& ecdh_private);

  /// Run the server side of the handshake.
  alert_location answer_handshake();

  //-----helper
  //currentALert
  alert_location currentAlert;
  void setCurrentAlert(alert_location currentAlert_);
  alert_location checkError();
  handshake_message_header returnClientHeader();
  std::vector<uint8_t> getClientPayload(size_t payload_length_);
  int num_32 = 32;

  Extension getKeyShare(std::vector<uint8_t> clientPayload);
  KeyShareEntry getKeyShareEntry(std::vector<uint8_t> clientPayload);
  std::vector<uint8_t> keyForClient;
    std::vector<uint8_t> getKeyForClient();
    void setKeyForClient(std::vector<uint8_t> keyForClient_);
    void setKeyForClient(std::vector<uint8_t> clientPayload, uint16_t size_of_key);
    size_t currentPosition = 0;
    void setCurrentPosition(size_t size);
    Extension getExtension(std::vector<uint8_t> clientPayload);
    Extension setPskEx(std::vector<uint8_t> clientPayload);
    size_t getSizeOfIdentites(std::vector<uint8_t> clientPayload, size_t iterator);
    cipher_suite selected_cipher_suite;
    std::vector<uint8_t> clientHeader;
    void setClientHeader(std::vector<uint8_t> clientHeader_);
    std::vector<uint8_t> serverMessagesVector;
    uint16_t  ident_selected = 0;

    size_t  serverHelloMessageSize = 0;
    void setSereverHelloMessageSize(size_t size);

    size_t  extensionSize = 0;
    void setExtenesionSize(size_t size);


    std::vector<uint8_t > vectorForSending();

    size_t getLengthForClientHash(handshake_message_header handshakeMessageHeader);

private:
  alert_location read_client_hello();
  void send_server_hello();
  alert_location read_finished();
  void send_finished();

  tls_record_layer& layer_;
  const psk_map psks_;

  ecdh ecdh_;
  const random_struct fixed_randomness_;
  const bool have_fixed_randomness_;

};

#endif // TLS_HANDSHAKE_SERVER_H
