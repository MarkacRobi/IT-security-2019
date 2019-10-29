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
  int it = 0;
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
                               const std::vector<uint8_t>& vectorParamSecond, int xorValue = 0)
{  
  if(xorValue == 0)
    xorValue = 1;

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

std::size_t caclXbit(size_t i, const std::vector<uint8_t>&X)
{
  return ((((uint8_t)X[i/8]) >> (7 - i % 8)) & 1);
}

bool isXbitZero(size_t i, const std::vector<uint8_t>&X)
{
  size_t xBit = caclXbit(i, X);
  if(xBit == 0)
    return true;
  else
    return false;
}

uint8_t calLeastSignificantBit(const std::vector<std::vector<uint8_t>>&V_0, size_t i)
{
  return (V_0[i][15] & 1);
}

std::vector<uint8_t> mult_of_two_blocks(const std::vector<uint8_t>&X, const std::vector<uint8_t>&Y){
    std::vector<uint8_t> R(16, 00000000);
    std::vector<std::vector<uint8_t>>Z(129);
    R[0] = 225;

    Z[0].push_back(0);//da li treba for??
    std::vector<std::vector<uint8_t>>V_0;
    V_0[0] = transferVectorData(Y);

    for(size_t i = 0; i < 128; i++)
    {
      if(isXbitZero(i, X))
        Z[i + 1] = transferVectorData(Z[i]);
      else
        Z[i + 1] = changeVectorData(Z[i], V_0[i], caclXbit(i, X));

     uint8_t vi_lsb = calLeastSignificantBit(V_0, i);

      std::vector<uint8_t>temp_array(V_0[i]);
      if(vi_lsb != 0)
        temp_array = changeVectorData(moveToRightSide(temp_array), R, vi_lsb);

      else
        temp_array = transferVectorData(moveToRightSide(temp_array));
      
      V_0[i + 1] = temp_array;
    }
    
    std::vector<uint8_t> return_vector = Z[128];
    return return_vector;
}

std::vector <uint8_t> g_hash(const std::vector<uint8_t>&H, const std::vector<std::vector<uint8_t>>&X)
{
  std::vector<std::vector<uint8_t>>Y_0(129);//??
  Y_0[0].push_back(0);//for
  
  size_t i = 0;
  while(i < X.size())
  { 
    std::vector<uint8_t> temp_array = changeVectorData(Y_0[i], X[i]);
    Y_0[++i] = mult_of_two_blocks(temp_array, H);//??
    
  }

  return Y_0[X.size()];
}

std::size_t getN(size_t X_length)
{
  return std::ceil((float) X_length / 16.0);//?? / 128
}

std::vector<uint8_t> calcXi(size_t i, const std::vector<uint8_t>&X)
{
  size_t index = 16 * i;
  std::vector<uint8_t>temp_vector;
  while(index < (16*(i + 1)))
  { 
    if(index == X.size())
      break;
    temp_vector.push_back(X[index]);
    index++;
  }

  return temp_vector;
}

std::vector<uint8_t> calcYn(const std::vector<std::vector<uint8_t>> Y)
{ 
  std::vector<uint8_t> return_result;
  size_t i = 0;
  while(i < Y.size())
  { 
    size_t j = 0;
    while(j < Y[i].size())
    { 
      return_result.push_back(Y[i][j]);  
      j++;
    }
    i++;
  }

  return return_result;
}

std::vector<uint8_t> gctr(const std::array<uint8_t, 16>&K, const std::vector<uint8_t>&C_1, const std::vector<uint8_t>&X)
{ 

  aes128 aes128_(K.data());
  std::vector<uint8_t> Ek_Cn(16);
  std::vector<uint8_t> Ml;

  std::vector<std::vector<uint8_t>> X_for_xor;

  std::vector<uint8_t> gctr_return;
  size_t X_length = X.size();
  if(X_length == 0)
    return gctr_return;

  std::vector<std::vector<uint8_t>> Y;
  std::vector<std::vector<uint8_t>> Ci;
  Ci.push_back(C_1);

  size_t n = getN(X_length);
  for(size_t i = 0; i < n; i++)
  {
    if(i != 0)
    { 
      std::vector<uint8_t> temp = calcVectorHex(Ci[i - 1]);
      Ci.push_back(temp);
    }
  }

  

  for(size_t i = 0; i < n; i++)
  {
    std::vector<uint8_t>temp_vector;
    temp_vector = calcXi(i, X);
    X_for_xor.push_back(temp_vector);
  }

  for(size_t i = 0; i < n - 1; i++)
  {
    std::vector<uint8_t> Ek(16);
    aes128_.encrypt(Ek.data(), Ci[i].data());
    std::vector<uint8_t> temp_vector_for_xor;
    temp_vector_for_xor = changeVectorData(X_for_xor[i], Ek);
    Y.push_back(temp_vector_for_xor);
  }

  aes128_.encrypt(Ek_Cn.data(), Ci[n - 1].data());
  size_t j = 0;
  while(j < X_for_xor[n - 1].size())
  { 
    Ml.push_back(Ek_Cn[j]);
    j++;
  }

  std::vector<uint8_t> temp_xor_array;
  temp_xor_array = changeVectorData(X_for_xor[n - 1], Ml);
  Y.push_back(temp_xor_array);

  gctr_return = calcYn(Y);
  return gctr_return;
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
