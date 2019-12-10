// Some functions here (e.g., the additions with the if clauses) are not constant-time. However, they can easily be made constant-time with small modifications.
// Compile with g++ -std=c++0x -g -O2 -march=native -mtune=native <...> -lcrypto

#include "BigInteger.h"
#include <math.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <iostream>
#include <fstream>


void BigInteger::AddIntegers(word* c, word* a, word* b, word* carry, uint32 num_words_operands) {
  // TODO: To implement
}

void BigInteger::SubtractIntegers(word* c, word* a, word* b, word* borrow, uint32 num_words_operands) {
  // TODO: To implement
}

void BigInteger::MultiplyIntegers(word* c, word* a, word* b, uint32 num_words_operands) {
  // TODO: To implement
}

void BigInteger::KaratsubaOfman(word* c, word* a, word* b, uint32 num_words_operands) {
  // TODO: To implement
}


bool BigInteger::SmallerThan(word* a, word* b, uint32 num_words_a, uint32 num_words_b) {
  // TODO: To implement
  return false;
}

bool BigInteger::GreaterThan(word* a, word* b, uint32 num_words_a, uint32 num_words_b) {
  // TODO: To implement
  return false;
}
