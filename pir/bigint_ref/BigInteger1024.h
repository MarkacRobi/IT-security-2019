// Some functions here (e.g., the additions with the if clauses) are not constant-time. However, they can easily be made constant-time with small modifications.

#ifndef BIGINTEGER1024_H
#define BIGINTEGER1024_H

#include "types.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

#define NUM_WORDS_1024 16
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
  void WriteData(uchar*);
  bool GreaterThanModulus(uint32) const;

  // Operators
  friend BigInteger1024 operator+(const BigInteger1024& a, const BigInteger1024& b);
  friend BigInteger1024 operator-(const BigInteger1024& a, const BigInteger1024& b);
  friend BigInteger1024 operator*(const BigInteger1024& a, const BigInteger1024& b);
  BigInteger1024& operator=(const BigInteger1024& other);
  friend std::ostream& operator<<(std::ostream& output, const BigInteger1024& bigint);
  bool operator==(const BigInteger1024& rhs) const;
  bool operator!=(const BigInteger1024& rhs) const;
  bool operator<(const BigInteger1024& rhs) const;
  bool operator>(const BigInteger1024& rhs) const;
  bool operator<=(const BigInteger1024& rhs) const;
  bool operator>=(const BigInteger1024& rhs) const;

  // Static Helpers
  BigInteger1024 static PowerMod(const BigInteger1024&, const BigInteger1024&);
  BigInteger1024 static Inverse(const BigInteger1024&);
  static std::vector<BigInteger1024> FromFile(const std::string&);
  BigInteger1024 GetPhiM()const ;
  void ShiftOneBitRight();
  static std::string GetString(const BigInteger1024&);
  BigInteger1024 static Mod(const BigInteger1024&, const BigInteger1024&);
  BigInteger1024 static GCD(const BigInteger1024&, const BigInteger1024&);
  BigInteger1024 static LCM(const BigInteger1024&, const BigInteger1024&);

private:
  uint32 num_bytes_;
  uchar* data_;
  uchar* modulus_;
};

#endif
