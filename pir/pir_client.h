//
// Created by Edi Muskardin on 06/11/2019.
//

#ifndef TEST_CLIENTSERVERSPIR_H
#define TEST_CLIENTSERVERSPIR_H

#include <cstdint>
#include <vector>
#ifdef PROTOCOLS
#include "bigint_ref/BigInteger1024.h"
#include "bigint_ref/BigInteger2048.h"
#else
#include "bigint/BigInteger1024.h"
#include "bigint/BigInteger2048.h"
#endif
#include "utils.h"
#include "paillier.h"

class pir_client {
private:
    uint8_t numServers;
    uint8_t desiredPrivacy;
    uint8_t numImages;
    const uint32_t imageSize = 19968; // exact size of each image in bytes
    const uint8_t blockSize = 127;
    const uint16_t blocksPerImage = imageSize / blockSize;
public:
    pir_client(uint8_t nServers, uint8_t dPrivacy, uint8_t numImgs) : numServers(nServers), desiredPrivacy(dPrivacy), numImages(numImgs) {};
    void setUpClient();
    std::vector<std::vector<BigInteger2048>> clientQuery(uint8_t imageIndex);
    std::vector<BigInteger1024> clientFinish(std::vector<std::vector<BigInteger2048>>& serverResponses);
};

#endif //TEST_CLIENTSERVERSPIR_H
