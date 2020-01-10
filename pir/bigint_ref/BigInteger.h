// Some functions here (e.g., the additions with the if clauses) are not constant-time. However, they can easily be made constant-time with small modifications.

#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include "types.h"
#include <iostream>
#include <sstream>
#include <iomanip>

#define NUM_WORDS_2048 32
#define NUM_BYTES_2048 256

class BigInteger
{
public:
  // Static Helpers
  std::string static ToString(void*, uint32);
  void static AddIntegers(word*, word*, word*, word*, uint32);
  void static SubtractIntegers(word*, word*, word*, word*, uint32);
  void static MultiplyIntegers(word*, word*, word*, uint32);
  void static KaratsubaOfman(word*, word*, word*, uint32);
  void static ShiftLeftByBytes(uchar*, uchar*, uint32, uint32);
  void static ShiftRightByBytes(uchar*, uchar*, uint32, uint32);
  void static BitwiseAnd(word*, word*, word*, uint32);
  bool static Equal(word*, word*, uint32);
  bool static SmallerThan(word*, word*, uint32, uint32);
  bool static GreaterThan(word*, word*, uint32, uint32);
};

#endif
