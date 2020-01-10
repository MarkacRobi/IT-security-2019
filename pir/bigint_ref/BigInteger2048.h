// Some functions here (e.g., the additions with the if clauses) are not constant-time. However, they can easily be made constant-time with small modifications.

#ifndef BIGINTEGER2048_H
#define BIGINTEGER2048_H

#include "types.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "BigInteger1024.h"

#define NUM_WORDS_2048 32
#define NUM_BYTES_2048 256

class BigInteger2048
{
public:
  BigInteger2048(uint64 init_value = 0);
  BigInteger2048(const BigInteger1024&);
  BigInteger2048(const BigInteger2048&);
  BigInteger2048(const std::string hex_string);
  ~BigInteger2048();

  void Randomize();
  uchar* GetData() const;
  uchar* GetModulus() const;
  uint32 GetNumBytes() const;
  void WriteData(uchar*);
  bool GreaterThanModulus(uint32) const;
  void ShiftOneBitRight();

  // Operators
  friend BigInteger2048 operator+(const BigInteger2048& a, const BigInteger2048& b);
  friend BigInteger2048 operator-(const BigInteger2048& a, const BigInteger2048& b);
  friend BigInteger2048 operator*(const BigInteger2048& a, const BigInteger2048& b);
  friend BigInteger1024 operator/(const BigInteger2048& a, const BigInteger1024& b);
  BigInteger2048& operator=(const BigInteger2048& other);
  friend std::ostream& operator<<(std::ostream& output, const BigInteger2048& bigint);
  bool operator==(const BigInteger2048& rhs) const;
  bool operator!=(const BigInteger2048& rhs) const;
  bool operator<(const BigInteger2048& rhs) const;
  bool operator>(const BigInteger2048& rhs) const;
  bool operator<=(const BigInteger2048& rhs) const;
  bool operator>=(const BigInteger2048& rhs) const;

  // Static Helpers
  BigInteger2048 static PowerMod(const BigInteger2048& a, const BigInteger2048& b);
  static std::vector<BigInteger2048> FromFile(const std::string &filename);

private:
  uint32 num_bytes_;
  uchar* data_;
  uchar* modulus_;
};

#endif
