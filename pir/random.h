// Some functions here (e.g., the additions with the if clauses) are not constant-time. However, they can easily be made constant-time with small modifications.

#ifndef RANDOM_H
#define RANDOM_H

#include "bigint/types.h"
#include "../tls/aes/aes128.h"

class Random
{
public:
  Random() = delete;
  // Static Helpers
  void static Init(uchar*);
  void static WriteRandomBytes(uchar*, uint32);
private:
  static aes128 AES_engine;
  static uchar state[16];
};

#endif
