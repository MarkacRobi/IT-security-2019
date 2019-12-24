#ifndef PIR_UTILS_H
#define PIR_UTILS_H

#include <cstdint>
#include <cstddef>
#include <string>
#include "bigint/BigInteger1024.h"

namespace utils{
    std::vector<BigInteger1024> getFile(uint8_t fileIndex, uint16_t blocksPerImage);
    std::vector<std::vector<BigInteger1024>> getAllImages(std::string pathToDatabase, uint8_t numImages, uint16_t blocksPerImage);
    void writeToFile(const std::vector<BigInteger1024 >& data, const std::string& filename);
    void executeProtocol(uint8_t numServers, uint8_t desiredPriv, uint8_t numImages, uint8_t imageIndex, const std::string& resultFileName);
}

#endif
