// Some functions here (e.g., the additions with the if clauses) are not constant-time. However, they can easily be made constant-time with small modifications.

#ifndef BIGINTEGER1024_H
#define BIGINTEGER1024_H

#include "types.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

// TODO: Find suitable value
#define NUM_WORDS_1024 1
#define NUM_BYTES_1024 128

class BigInteger1024
{
public:
  BigInteger1024(uint64 init_value = 0);
  BigInteger1024(const BigInteger1024&);
  BigInteger1024(const std::string hex_string);
  ~BigInteger1024();

  void Randomize();
  uchar* GetData() const;
  uchar* GetModulus() const;
  uint32 GetNumBytes() const;

  // Operators
  friend BigInteger1024 operator+(const BigInteger1024& a, const BigInteger1024& b);
  friend BigInteger1024 operator-(const BigInteger1024& a, const BigInteger1024& b);
  friend BigInteger1024 operator*(const BigInteger1024& a, const BigInteger1024& b);

  // Static Helpers
  BigInteger1024 static PowerMod(const BigInteger1024&, const BigInteger1024&);
  BigInteger1024 static Inverse(const BigInteger1024&);
  static std::vector<BigInteger1024> FromFile(const std::string&);

private:
  uint32 num_bytes_;
  uchar* data_;
  uchar* modulus_;
};

#endif
