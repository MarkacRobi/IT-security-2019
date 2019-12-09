// Some functions here (e.g., the additions with the if clauses) are not constant-time. However, they can easily be made constant-time with small modifications.
// Compile with g++ -std=c++0x -g -O2 -march=native -mtune=native <...> -lcrypto

#include "random.h"
#include <math.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <iostream>
#include <fstream>

uchar Random::state[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
aes128 Random::AES_engine;

void Random::Init(uchar* seed) {
  Random::AES_engine.set_key((uint8_t*)seed);
  for(uchar i = 0; i < 16; i++)
  {
      state[i] = i;
  }
}

void Random::WriteRandomBytes(uchar* destination, uint32 num_blocks) {
  while(num_blocks--) {
    // Writes from MSB to LSB
    Random::AES_engine.encrypt((uint8_t*)destination + (num_blocks * 16), (uint8_t*)state);
    uint64* ptr = (uint64*)(Random::state);
    *ptr += 1;
  }
}
