#include "pir_client.h"
#include "shamir.h"
#include "pir_server.h"

void pir_client::setUpClient() {
    // TODO set up client
}


std::vector<std::vector<BigInteger2048>> pir_client::clientQuery(uint8_t imageIndex) {
    // TODO create shares which will be send to servers
    return std::vector<std::vector<BigInteger2048>>();

}

std::vector<BigInteger1024> pir_client::clientFinish(std::vector<std::vector<BigInteger2048>>& serverResponses) {
    // TODO finish the protocol by reconstructing image with shares received from servers
    return std::vector<BigInteger1024>();

}
