#include "aes128gcm.h"
#include "aes/aes128.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <bitset>

#define  HEX1 0xff00000000000000
#define  HEX2 0xff000000000000
#define  HEX3 0xff0000000000
#define  HEX4 0xff00000000
#define  HEX5 0xff000000
#define  HEX6 0xff0000
#define  HEX7 0xff00
#define  HEX8 0xff
#define  OR 0x80

// Take care to program the GCM specific functions in constant time,
// meaning that no conditional branches or conditional loads that depend
// on the key, the nonce or the data are allowed. This means that the
// program flow should be fully independent from the input data.
// Do not make any assumptions on the cache line sizes and stack alignment.


aes128gcm::aes128gcm(){
/// \todo Initialize with an all 0 key.

  int sizeOfHolder = key_holder.size();
  unsigned int it = 0;
  while(it < sizeOfHolder)
  {
    key_holder[it] = 0;
    it++;
  }
}

aes128gcm::aes128gcm(const key_storage& key)
{
  /// \todo Initialise with the given key.
    int sizeOfHolder = key.size();
  unsigned int it = 0;
  while(it < sizeOfHolder)
  {
    key_holder[it] = key[it];
    it++;
  }

}

std::vector<uint8_t> fillVector(const std::vector<uint8_t>& vectorInput)
{
  std::vector<uint8_t> vectorForMaxInput;
  int sizeOfVector = vectorInput.size() - 4;
  unsigned long ctr = 0;
  while(ctr < sizeOfVector)
  {
        vectorForMaxInput.push_back(vectorInput[ctr]);
        ctr++;
  }
  return vectorForMaxInput;
}
void aes128gcm::set_key(const key_storage& key)
{
  /// \todo Reset the key.

     int sizeOfHolder = key.size();
  unsigned int it = 0;
  while(it < sizeOfHolder)
  {
    key_holder[it] = key[it];
    it++;
  }
}
std::vector<uint8_t> pushBackIntoVector(std::vector<uint8_t> vectorForMaxInput, uint8_t temp)
{
  vectorForMaxInput.push_back(temp & HEX5);

  vectorForMaxInput.push_back(temp & HEX6);

  vectorForMaxInput.push_back(temp & HEX7);

  vectorForMaxInput.push_back(temp & HEX8);

  return vectorForMaxInput;
}

std::vector<uint8_t> calcVectorHex(const std::vector<uint8_t>& vectorInput)
{
  
  std::vector<uint8_t> vectorForMaxInput = fillVector(vectorInput);
  uint8_t  temp = (vectorInput[vectorInput.size() - 4] << 24) 
                + (vectorInput[vectorInput.size() - 3] << 16) + 
                (vectorInput[vectorInput.size() - 2] << 8) 
                + vectorInput[vectorInput.size() - 1];
  temp = temp +1;

  vectorForMaxInput = pushBackIntoVector(vectorForMaxInput, temp);

  return vectorForMaxInput;
}


std::vector<uint8_t> moveToRightSide(const std::vector<uint8_t>& vectorP)
{
  std::vector<uint8_t> vectorForMaxInput;

  uint8_t firstFromVector = 0;

  auto transferBit = 0;

  unsigned int counter = 0;

  int sizeOfArray = vectorP.size();

  while(counter < sizeOfArray)
  {
    firstFromVector = vectorP[counter];
    firstFromVector >>= 1;

    if(transferBit == 1)
    {
        firstFromVector |= OR;
    }

    auto resultingOcc= vectorP[counter] % 2;
    if(resultingOcc != 0)
    {
        transferBit = 1;
    }
    else
    {
      transferBit = 0;
    }

    vectorForMaxInput.push_back(firstFromVector);
         
    counter++;
  }
  return vectorForMaxInput;
}  

std::vector<uint8_t> transferVectorData(const std::vector<uint8_t>& vectorParam)
{
  std::vector<uint8_t> vectorForMaxInput;

  unsigned int counter = 0;

  int sizeOfArray = vectorParam.size();

  while(counter < sizeOfArray)
  {
    vectorForMaxInput.push_back(vectorParam[counter]);

    counter++;

  }
  return vectorForMaxInput;
}

std::vector<uint8_t> changeVectorData(const std::vector<uint8_t>& vectorParam, 
                               const std::vector<uint8_t>& vectorParamSecond, int xorValue = 1)
{
  std::vector<uint8_t> vectorForMaxInput;

  unsigned int counter = 0;
  int sizeOfArray = vectorParam.size();

  while(counter < sizeOfArray)
  {
    vectorForMaxInput.push_back(vectorParam[counter] ^ 
                                (vectorParamSecond[counter] 
                                * xorValue));
    counter++;

  }
  return vectorForMaxInput;
}

bool aes128gcm::encrypt(std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& plaintext,
                        const std::vector<uint8_t>& nonce_data,
                        const std::vector<uint8_t>& additional_data) const
{
  /// \todo Encrypt plaintext using AES-GCM with the given nonce and additional data.
  return false;
}

bool aes128gcm::decrypt(std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& ciphertext,
                        const std::vector<uint8_t>& nonce_data,
                        const std::vector<uint8_t>& additional_data) const
{
  /// \todo Decrypt ciphertext using AEs-GCM with the given nonce and additional data.
  return false;
}
