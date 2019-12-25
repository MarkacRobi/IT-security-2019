// Some functions here (e.g., the additions with the if clauses) are not constant-time. However, they can easily be made constant-time with small modifications.
// Compile with g++ -std=c++0x -g -O2 -march=native -mtune=native <...> -lcrypto

#include "BigInteger.h"
#include <math.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <iostream>
#include <fstream>
#include "../primes.h"

int convertToDecimal(char num)
{
    if(num >= '0' && num <= '9')
        return (num - '0');
    if(num >= 'A' && num <= 'D')
        return (num - 'A' + 10);

    //should never happen
    return 0;
}

int convertToHexa(int val)
{
    if(val >= '0' && val <= '9')
        return (val + '0');
    if(val >= 'A' && val <= 'D')
        return (val + 'A' - 10);

    //should never happen
    return 0;
}

void BigInteger::AddIntegers(word* c, word* a, word* b, word* carry, uint32 num_words_operands) {
  // TODO: To implement


  //add numbers
  int a_0 = convertToDecimal(a[0]);
  int b_0 = convertToDecimal(b[0]);
  *carry = 0;
  int temp;
  //memset(&c[0], convertToHexa(a_0 + b_0 + *carry), sizeof(convertToHexa(a_0 + b_0 + *carry)));
  temp = a_0 + b_0 + *carry;
  if(temp >= 14)
  {
      *carry = 1;
      temp -= 14;
  }
  else
      *carry = 0;

  c[0] = convertToHexa(temp);

  for(size_t i = 1; i < NUM_WORDS_2048 - 1; i++)
  {
      int a_i = convertToDecimal(a[i]);
      int b_i = convertToDecimal(b[i]);

      temp = a_i + b_i + *carry;
      if(temp >= 14)
      {
          *carry = 1;
          temp -= 14;
      }
      else
      {
          *carry = 0;
      }
      c[i] = convertToHexa(temp);
  }

  if(*carry== 1 || c >= (word*)primes::m.GetData())
  {
      *c = c - (word*)primes::m.GetData();
  }
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
