#include "utils.h"
#include "bigint/BigInteger.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include "pir_client.h"
#include "pir_server.h"

namespace utils{

    std::vector<BigInteger1024 > getFile(std::string pathToDatabase, uint8_t fileIndex, uint16_t blocksPerImage) {
        if(fileIndex > 9)
            throw std::invalid_argument("Image indexes are in range [0,9]");
        std::string fileName = pathToDatabase;
        fileName.append("img");
        fileName.append(std::to_string(fileIndex));
        fileName.append(".jpg");

        std::ifstream file(fileName, std::ios::binary);
        std::vector<BigInteger1024> fileData;

        for(uint16_t i = 0; i < blocksPerImage; i++){
            unsigned char buffer[127];
            file.read((char*) buffer, 127);
            std::stringstream ss;
            for(unsigned char j : buffer)
                ss << std::hex << std::setw(2) << std::setfill('0') << (int) j;
            BigInteger1024 e(std::string(2, '0').append(ss.str()));
            fileData.emplace_back(e);
        }
        return fileData;
    }

    void writeToFile(const std::vector<BigInteger1024 >& data, const std::string& filename){
        std::ofstream file(filename, std::ios_base::binary);
        for (const auto &e : data){
          const uchar* e_d = e.GetData();
          for(int i = 0; i < 127; ++i)
             file << e_d[NUM_BYTES_1024 - i - 2];
        }
    }

    std::vector<std::vector<BigInteger1024>> getAllImages(std::string pathToDatabase, uint8_t numImages, uint16_t blocksPerImage) {
        std::vector<std::vector<BigInteger1024>> database;
        database.reserve(numImages);
        for(uint8_t i = 0; i < numImages; i++)
            database.emplace_back(getFile(pathToDatabase, i, blocksPerImage));
        return database;
    }

    void executeProtocol(uint8_t numServers, uint8_t desiredPrivacy, uint8_t numImages, uint8_t imageIndex, const std::string& resultFileName){
        pir_client client(numServers, desiredPrivacy, numImages);
        pir_server server(numImages, 19968/127);
        client.setUpClient();
        auto encPi = client.clientQuery(imageIndex);
        std::vector<std::vector<BigInteger2048>> serverResponses;
        serverResponses.reserve(encPi.size());
        for(auto &e : encPi)
            serverResponses.push_back(server.serverResponse(e));
        auto reconstructedImage = client.clientFinish(serverResponses);
        utils::writeToFile(reconstructedImage, resultFileName);
    }
}
