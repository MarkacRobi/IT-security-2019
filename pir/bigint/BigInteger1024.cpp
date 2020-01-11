// Some functions here (e.g., the additions with the if clauses) are not constant-time. However, they can easily be made constant-time with small modifications.
// Compile with g++ -std=c++0x -g -O2 -march=native -mtune=native <...> -lcrypto

#include "BigInteger1024.h"
#include "BigInteger2048.h"
#include "BigInteger.h"
#include "../random.h"
#include "../primes.h"
#include <sstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

BigInteger1024::BigInteger1024(uint64 init_value) {
  // ASSUME num_bits mod WORD_SIZE = 0
  this->num_bytes_ = NUM_BYTES_1024;
  this->data_ = new uchar[NUM_BYTES_1024];
  this->modulus_ = new uchar[NUM_BYTES_1024];

  memset(this->data_, 0x0, this->num_bytes_);

  // Fix modulus here
  uint64* modulus_words = (uint64*)(this->modulus_);
  modulus_words[0] = 0x7b777e286ffcecf1;
  modulus_words[1] = 0x3aec4129691f5ae4;
  modulus_words[2] = 0xde442a9cdb49b8b5;
  modulus_words[3] = 0x1773ac484b960201;
  modulus_words[4] = 0x6f3cb98d2fd296e8;
  modulus_words[5] = 0x3eb3abf93d157b43;
  modulus_words[6] = 0x35e7b392e86e0c66;
  modulus_words[7] = 0xe00de5741dc2d023;
  modulus_words[8] = 0x6f3ca01d26219cf8;
  modulus_words[9] = 0x2bd21a518028b0ee;
  modulus_words[10] = 0x6b7bd175b22b1659;
  modulus_words[11] = 0xa9bd2b39b3cd083e;
  modulus_words[12] = 0x2a182a206c1b166f;
  modulus_words[13] = 0x1aaee8610dcd9347;
  modulus_words[14] = 0x7ad3b837bc304933;
  modulus_words[15] = 0xad7ea541780f4050;

  memcpy(this->data_, &init_value, 8);
  // Init
}

BigInteger1024::BigInteger1024(const BigInteger1024& bigint) {
  this->num_bytes_ = bigint.GetNumBytes();
  this->data_ = new uchar[this->num_bytes_];
  this->modulus_ = new uchar[this->num_bytes_];
  memcpy(this->data_, bigint.GetData(), this->num_bytes_);
  memcpy(this->modulus_, bigint.GetModulus(), this->num_bytes_);
}

BigInteger1024::BigInteger1024(const std::string hex_string){

  this->num_bytes_ = NUM_BYTES_1024;
  this->data_ = new uchar[NUM_BYTES_1024];
  this->modulus_ = new uchar[NUM_BYTES_1024];

  memset(this->data_, 0x0, this->num_bytes_);

  // Fix modulus here
  uint64* modulus_words = (uint64*)(this->modulus_);
  modulus_words[0] = 0x7b777e286ffcecf1;
  modulus_words[1] = 0x3aec4129691f5ae4;
  modulus_words[2] = 0xde442a9cdb49b8b5;
  modulus_words[3] = 0x1773ac484b960201;
  modulus_words[4] = 0x6f3cb98d2fd296e8;
  modulus_words[5] = 0x3eb3abf93d157b43;
  modulus_words[6] = 0x35e7b392e86e0c66;
  modulus_words[7] = 0xe00de5741dc2d023;
  modulus_words[8] = 0x6f3ca01d26219cf8;
  modulus_words[9] = 0x2bd21a518028b0ee;
  modulus_words[10] = 0x6b7bd175b22b1659;
  modulus_words[11] = 0xa9bd2b39b3cd083e;
  modulus_words[12] = 0x2a182a206c1b166f;
  modulus_words[13] = 0x1aaee8610dcd9347;
  modulus_words[14] = 0x7ad3b837bc304933;
  modulus_words[15] = 0xad7ea541780f4050;

  //init from hex
  std::stringstream ss;
  word* data = (word*) this->data_;

  if(hex_string.size() / 2 != NUM_BYTES_1024)
    throw std::invalid_argument("String must represent a 1024-bit integer!");


  for(size_t i = 0; i < NUM_WORDS_1024; i++){
    ss.clear();

    //printf("position1 is: %d\n", i * sizeof(word) * 2);
      //printf("position is: %d\n", sizeof(word) * 2);
    ss << std::hex << std::setfill('0') << std::setw(2) << hex_string.substr(i * sizeof(word) * 2, sizeof(word) * 2);

    word current;
    ss >> current;
    size_t index = (NUM_WORDS_1024 - 1) - i;
    data[index] = current;
  }
//
//  printf("data is \n");
//  for(size_t i = 0; i < 16; i++)
//      printf("%x", this->GetData()[i]);
//
//  printf("\n");
}

BigInteger1024::~BigInteger1024() {
  delete[] this->data_;
  delete[] this->modulus_;
}

void BigInteger1024::Randomize() {
  // AES CTR
  uint32 num_blocks = (this->num_bytes_ >> 4);
  Random::WriteRandomBytes(this->data_, num_blocks);
  while(!(BigInteger::SmallerThan((word*)(this->data_), (word*)(this->modulus_), NUM_WORDS_1024, NUM_WORDS_1024))) {
    Random::WriteRandomBytes(this->data_, num_blocks);
  }
}

uchar* BigInteger1024::GetData() const {
  return this->data_;
}

uchar* BigInteger1024::GetModulus() const {
  return this->modulus_;
}

uint32 BigInteger1024::GetNumBytes() const {
  return this->num_bytes_;
}


BigInteger1024 operator+(const BigInteger1024& a, const BigInteger1024& b) {
  // TODO: To implement
  BigInteger2048  c;
  BigInteger::AddIntegers((word*)c.GetData(), (word*)a.GetData(), (word*)b.GetData(), (word*)(c.GetData() + NUM_BYTES_1024), NUM_WORDS_1024);

    word *carry = (word*)(c.GetData() + NUM_BYTES_1024);
    if(*carry == 1
       || BigInteger::GreaterThan((word*)c.GetData(), (word*)primes::m.GetModulus(), NUM_WORDS_1024, NUM_WORDS_1024)
       || ((BigInteger::GreaterThan((word*)c.GetData(), (word*)primes::m.GetModulus(), NUM_WORDS_1024, NUM_WORDS_1024)) == false &&
           (BigInteger::SmallerThan((word*)c.GetData(), (word*)primes::m.GetModulus(), NUM_WORDS_1024, NUM_WORDS_1024)) == false))
    {
        printf("oduzmi sa modulus1024\n");
        BigInteger::SubtractIntegers((word*)c.GetData(), (word*)c.GetData(), (word*)primes::m.GetModulus(), (word*)(c.GetData() + NUM_BYTES_1024), NUM_WORDS_1024);
    }


  BigInteger1024 res;


  for(int i = 0; i < NUM_BYTES_1024; i++)
  {
      res.GetData()[i] = c.GetData()[i];
  }

  return res;
}

BigInteger1024 operator-(const BigInteger1024& a, const BigInteger1024& b) {
  // TODO: To implement
    BigInteger2048  c;
    BigInteger::SubtractIntegers((word*)c.GetData(), (word*)a.GetData(), (word*)b.GetData(), (word*)(c.GetData() + NUM_BYTES_1024), NUM_WORDS_1024);

    word *borrow = (word*)(c.GetData() + NUM_BYTES_1024);
    if(*borrow == 1)
    {
        printf("oduzmi sa modulus1024\n");
        printf("%x\n", primes::m.GetModulus());
        BigInteger::AddIntegers((word*)c.GetData(), (word*)c.GetData(), (word*)primes::m.GetModulus(), (word*)(c.GetData() + NUM_BYTES_1024), NUM_WORDS_1024);
    }

    BigInteger1024 res;

    for(int i = 0; i < NUM_BYTES_1024; i++)
    {
        res.GetData()[i] = c.GetData()[i];
    }

    return res;
}

BigInteger1024 operator*(const BigInteger1024& a, const BigInteger1024& b) {
  // TODO: To implement
  /*const std::string mu = ("0x107d4dc7c15d75295370e77aabf1c2ef0ff6deb6ea995a226c8dca38fd2e82e43ac54fa62085abf63d7798201ba124facbe4cb974dbaf72cd0e96863b79ce3af37d912421c2a18ae3b6215fbd0e5aa7459105a61e9804adb14aba3c966f20e8e36935db78cd08814cda1969510b0ed92dbd71d38b8aa5adbdf4742fb5157e1a17d5321ed548394c130496819943c2e90d4a74d73f38958232766a52309f2520b9194199bd80014a0c08d23a529d0f2a1fd933599301b8fb8941c52fa5701904c6039cab96f3e028554e3922f385eca64dc0e2a905682bd90f7b006fd23b5d057fcd6be7203d3f19d4d1733bba95ff5c9740044134e7c0c6ab510fc5abL");
   BigInteger2048 x;
    BigInteger1024 q;

    BigInteger::MultiplyIntegers((word*)x.GetData(), (word*)a.GetData(), (word*)b.GetData(), NUM_WORDS_1024);

    BigInteger1024 new_x;
    for(int i = 0; i < NUM_BYTES_1024; i++)
    {
        new_x.GetData()[i] = x.GetData()[i];
    }

    const std::string temp_b = "18446744073709551616"; //( 2 ^ 64)
    BigInteger1024 B = BigInteger1024(std::string(246, '0').append(temp_b));
     int number_of_times_to_shift = 4032;//log2(B)*(k-1)
    for(int i=0; i<NUM_BYTES_1024; i++)
    {
        mempcpy(new_x[i], number_of_times_to_shift, 2);
    }*/


  return BigInteger1024(0);
}


BigInteger1024 BigInteger1024::PowerMod(const BigInteger1024& a, const BigInteger1024& b) {
  // TODO: To implement
  BigInteger1024  power_result;
  for(int i = 0; i < NUM_BYTES_1024; i++)
  {

      if(a.GetData()[i] != 0)
      {
          printf("usooo\n");
          printf("%x\n", a.GetData()[i]);
          printf("%x\n", b.GetData()[i]);
          //power_result.GetData()[i] = pow(a.GetData()[i], b.GetData()[i]);
          long index = b.GetData()[i];
          int temp = 1;
          while(index != 0)
          {
              temp *= a.GetData()[i];
              index--;
              printf("---> %x\n", temp);
          }
          power_result.GetData()[i] = temp;
      }
  }

  return power_result;
}

bool checkIfModulusIsZero(BigInteger1024 b)
{
    bool isZero = true;
    for(int i = 0; i < NUM_WORDS_1024; i++)
    {
        if(b.GetData()[i] == 0)
            continue;
        else
        {
            isZero = false;
            break;
        }

    }

    return isZero;
}

BigInteger1024 static calcMod(BigInteger2048 a, BigInteger1024 b)
{
//    printf("b is ");
//        for(int i = 0; i < 128; i++)
//        printf("%x", b.GetModulus()[i]);
//    printf("\n");
//
//
//    printf("a is ");
//    for(int i = 0; i < 128; i++)
//        printf("%x", a.GetData()[i]);
//    printf("\n");


    BigInteger1024 res;
    for(int i = 0; i < 128; i++)
    {
        //printf("a %x\n", a.GetData()[i]);
        //printf("b %x\n", b.GetModulus()[i]);
        res.GetData()[i] = a.GetData()[i] % b.GetModulus()[i];
        //printf("res %x\n", res.GetData()[i]);
    }

    return res;
}

BigInteger1024 static ExtEuclidian(BigInteger2048 a, BigInteger1024 b)
{
    //    printf("modulus is ");
//    for(int i = 0; i < 64; i++)
//        printf("%x", b[i]);
//    printf("\n");
//
//
//    printf("a is ");
//    for(int i = 0; i < 64; i++)
//        printf("%x", a[i]);
//    printf("\n");


    //check if b == 0


    BigInteger1024 res = calcMod(a, b);

        printf("res is ");
    for(int i = 0; i < 128; i++)
        printf("%x", res.GetData()[i]);
    printf("\n");


    BigInteger1024 res2 = calcMod(b, res);

    printf("res2 is ");
    for(int i = 0; i < 128; i++)
        printf("%x", res2.GetData()[i]);
    printf("\n");


//    while(!checkIfModulusIsZero(calcMod(a, b)))
//    {
//        printf("nije nula\n");
//    }

    //printf("jeste nula\n");

    return BigInteger1024(0);

}



BigInteger1024 BigInteger1024::Inverse(const BigInteger1024& a) {
  // TODO: To implement
  BigInteger1024 b;//modulus
  BigInteger2048 a_ = BigInteger2048(a);//check this TODO

  BigInteger1024 inverse = ExtEuclidian(a_, b);

  return inverse;
}

std::vector<BigInteger1024> BigInteger1024::FromFile(const std::string &filename) {
    //std::cout<<"unutra sam" << std::endl;
  std::string line;
  std::ifstream file;
  file.open(filename);
  std::vector<BigInteger1024> result;

  while (std::getline(file, line))
  {
    result.emplace_back(BigInteger1024(line));
  }

  file.close();
  return result;
}

