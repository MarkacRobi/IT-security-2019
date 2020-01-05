// Some functions here (e.g., the additions with the if clauses) are not constant-time. However, they can easily be made constant-time with small modifications.
// Compile with g++ -std=c++0x -g -O2 -march=native -mtune=native <...> -lcrypto

#include "BigInteger2048.h"
#include "BigInteger.h"
#include "../random.h"
#include <sstream>
#include <iomanip>
#include <cstring>
#include <iostream>
#include <fstream>

BigInteger2048::BigInteger2048(uint64 init_value) {
  // ASSUME num_bits mod WORD_SIZE = 0
  this->num_bytes_ = NUM_BYTES_2048;
  this->data_ = new uchar[NUM_BYTES_2048];
  this->modulus_ = new uchar[NUM_BYTES_2048];
  memset(this->data_, 0x0, this->num_bytes_);

  // Fix modulus here
  uint64* modulus_words = (uint64*)(this->modulus_);
  modulus_words[0] = 0x75296ac53c53ae1;
  modulus_words[1] = 0x46701297c8de7ebc;
  modulus_words[2] = 0xb598a549203ae378;
  modulus_words[3] = 0x616feebdfdf4b6de;
  modulus_words[4] = 0xea185fa3844c02;
  modulus_words[5] = 0x341d390b6ada1fc6;
  modulus_words[6] = 0x10b9100f7266d8c;
  modulus_words[7] = 0x4d9c4837b84b9145;
  modulus_words[8] = 0xfe15b8ab1b05e931;
  modulus_words[9] = 0x2434cba0917155f6;
  modulus_words[10] = 0x504d20e26a611fa1;
  modulus_words[11] = 0x276d6845cb37cb80;
  modulus_words[12] = 0x9b19566103aa2762;
  modulus_words[13] = 0x895b1e2f44b061bf;
  modulus_words[14] = 0x571824ca4ee41de0;
  modulus_words[15] = 0xd7e60482d5fe8f6e;
  modulus_words[16] = 0x92a4ecfde2db6b70;
  modulus_words[17] = 0x99f4ede4412fe689;
  modulus_words[18] = 0x2deaefd299f6b976;
  modulus_words[19] = 0x8a199092b58de46a;
  modulus_words[20] = 0xe02a23b61e51e051;
  modulus_words[21] = 0x2a623ba0687dbb2c;
  modulus_words[22] = 0xd8b121d4ee4d8f50;
  modulus_words[23] = 0xe4ae0aedd8581b78;
  modulus_words[24] = 0x42a4a62df1a251ac;
  modulus_words[25] = 0x26be94842182343c;
  modulus_words[26] = 0x6bba1151de400381;
  modulus_words[27] = 0xae77c0651979613f;
  modulus_words[28] = 0xb4ff4b98422893c5;
  modulus_words[29] = 0xa192847d80275c26;
  modulus_words[30] = 0xee2fc3cc02ef1f95;
  modulus_words[31] = 0x75946a0193644171;

  // Init
  memcpy(this->data_, &init_value, 8);
}

BigInteger2048::BigInteger2048(const BigInteger1024& bigint) {
  this->num_bytes_ = NUM_BYTES_2048;
  this->data_ = new uchar[this->num_bytes_];
  this->modulus_ = new uchar[this->num_bytes_];
  memset(this->data_, 0x0, this->num_bytes_);
  memcpy(this->data_, bigint.GetData(), bigint.GetNumBytes());
  // Fix modulus here
  uint64* modulus_words = (uint64*)(this->modulus_);
  modulus_words[0] = 0x75296ac53c53ae1;
  modulus_words[1] = 0x46701297c8de7ebc;
  modulus_words[2] = 0xb598a549203ae378;
  modulus_words[3] = 0x616feebdfdf4b6de;
  modulus_words[4] = 0xea185fa3844c02;
  modulus_words[5] = 0x341d390b6ada1fc6;
  modulus_words[6] = 0x10b9100f7266d8c;
  modulus_words[7] = 0x4d9c4837b84b9145;
  modulus_words[8] = 0xfe15b8ab1b05e931;
  modulus_words[9] = 0x2434cba0917155f6;
  modulus_words[10] = 0x504d20e26a611fa1;
  modulus_words[11] = 0x276d6845cb37cb80;
  modulus_words[12] = 0x9b19566103aa2762;
  modulus_words[13] = 0x895b1e2f44b061bf;
  modulus_words[14] = 0x571824ca4ee41de0;
  modulus_words[15] = 0xd7e60482d5fe8f6e;
  modulus_words[16] = 0x92a4ecfde2db6b70;
  modulus_words[17] = 0x99f4ede4412fe689;
  modulus_words[18] = 0x2deaefd299f6b976;
  modulus_words[19] = 0x8a199092b58de46a;
  modulus_words[20] = 0xe02a23b61e51e051;
  modulus_words[21] = 0x2a623ba0687dbb2c;
  modulus_words[22] = 0xd8b121d4ee4d8f50;
  modulus_words[23] = 0xe4ae0aedd8581b78;
  modulus_words[24] = 0x42a4a62df1a251ac;
  modulus_words[25] = 0x26be94842182343c;
  modulus_words[26] = 0x6bba1151de400381;
  modulus_words[27] = 0xae77c0651979613f;
  modulus_words[28] = 0xb4ff4b98422893c5;
  modulus_words[29] = 0xa192847d80275c26;
  modulus_words[30] = 0xee2fc3cc02ef1f95;
  modulus_words[31] = 0x75946a0193644171;
}

BigInteger2048::BigInteger2048(const std::string hex_string){

    //std::cout << "hex is " << hex_string <<std::endl;
  this->num_bytes_ = NUM_BYTES_2048;
  this->data_ = new uchar[NUM_BYTES_2048];
  this->modulus_ = new uchar[NUM_BYTES_2048];

  memset(this->data_, 0x0, this->num_bytes_);

  // Fix modulus here
  uint64* modulus_words = (uint64*)(this->modulus_);
  modulus_words[0] = 0x75296ac53c53ae1;
  modulus_words[1] = 0x46701297c8de7ebc;
  modulus_words[2] = 0xb598a549203ae378;
  modulus_words[3] = 0x616feebdfdf4b6de;
  modulus_words[4] = 0xea185fa3844c02;
  modulus_words[5] = 0x341d390b6ada1fc6;
  modulus_words[6] = 0x10b9100f7266d8c;
  modulus_words[7] = 0x4d9c4837b84b9145;
  modulus_words[8] = 0xfe15b8ab1b05e931;
  modulus_words[9] = 0x2434cba0917155f6;
  modulus_words[10] = 0x504d20e26a611fa1;
  modulus_words[11] = 0x276d6845cb37cb80;
  modulus_words[12] = 0x9b19566103aa2762;
  modulus_words[13] = 0x895b1e2f44b061bf;
  modulus_words[14] = 0x571824ca4ee41de0;
  modulus_words[15] = 0xd7e60482d5fe8f6e;
  modulus_words[16] = 0x92a4ecfde2db6b70;
  modulus_words[17] = 0x99f4ede4412fe689;
  modulus_words[18] = 0x2deaefd299f6b976;
  modulus_words[19] = 0x8a199092b58de46a;
  modulus_words[20] = 0xe02a23b61e51e051;
  modulus_words[21] = 0x2a623ba0687dbb2c;
  modulus_words[22] = 0xd8b121d4ee4d8f50;
  modulus_words[23] = 0xe4ae0aedd8581b78;
  modulus_words[24] = 0x42a4a62df1a251ac;
  modulus_words[25] = 0x26be94842182343c;
  modulus_words[26] = 0x6bba1151de400381;
  modulus_words[27] = 0xae77c0651979613f;
  modulus_words[28] = 0xb4ff4b98422893c5;
  modulus_words[29] = 0xa192847d80275c26;
  modulus_words[30] = 0xee2fc3cc02ef1f95;
  modulus_words[31] = 0x75946a0193644171;

  //init from hex
  std::stringstream ss;
  word* data = (word*) this->data_;

  if(hex_string.size() / 2 != NUM_BYTES_2048)
    throw std::invalid_argument("String must represent a 2048 bit integer!");

  //printf("%d\n", NUM_WORDS_2048);
  for(size_t i = 0; i < NUM_WORDS_2048; i++){
    ss.clear();

    //printf("%d %d\n", i * sizeof(word) * 2, sizeof(word) * 2);
    //std::cout << hex_string.substr(i * sizeof(word) * 2, sizeof(word) * 2) << std::endl;
    ss << std::hex << std::setfill('0') << std::setw(2) << hex_string.substr(i * sizeof(word) * 2, sizeof(word) * 2);

    word current;
    ss >> current;
    size_t index = (NUM_WORDS_2048 - 1) - i;

    //printf("%x\n", current);
    data[index] = current;
  }

//     printf("data is \n");
// for(size_t i = 0; i < 256; i++)
//    printf("%x", this->GetData()[i]);

  printf("\n");
}

BigInteger2048::BigInteger2048(const BigInteger2048& bigint) {
    this->num_bytes_ = bigint.GetNumBytes();
    this->data_ = new uchar[this->num_bytes_];
    this->modulus_ = new uchar[this->num_bytes_];
    memcpy(this->data_, bigint.GetData(), this->num_bytes_);
    memcpy(this->modulus_, bigint.GetModulus(), this->num_bytes_);
}

BigInteger2048::~BigInteger2048() {
    delete[] this->data_;
    delete[] this->modulus_;
}

void BigInteger2048::Randomize() {
  // AES CTR
  uint32 num_blocks = (this->num_bytes_ >> 4);
  Random::WriteRandomBytes(this->data_, num_blocks);
  while(!(BigInteger::SmallerThan((word*)(this->data_), (word*)(this->modulus_), NUM_WORDS_2048, NUM_WORDS_2048))) {
    Random::WriteRandomBytes(this->data_, num_blocks);
  }
}

uchar* BigInteger2048::GetData() const {
  return this->data_;
}

uchar* BigInteger2048::GetModulus() const {
  return this->modulus_;
}

uint32 BigInteger2048::GetNumBytes() const {
  return this->num_bytes_;
}


BigInteger2048 operator+(const BigInteger2048& a, const BigInteger2048& b) {
  // TODO: To implement
    BigInteger2048  c;
    BigInteger::AddIntegers((word*)c.GetData(), (word*)a.GetData(), (word*)b.GetData(), (word*)(c.GetData() + NUM_BYTES_2048), NUM_WORDS_2048);

    return c;
}

BigInteger2048 operator-(const BigInteger2048& a, const BigInteger2048& b) {
  // TODO: To implement
  return BigInteger2048(0);
}

BigInteger2048 operator*(const BigInteger2048& a, const BigInteger2048& b) {
  // TODO: To implement
  return BigInteger2048(0);
}

BigInteger1024 operator/(const BigInteger2048& a, const BigInteger1024& b) {
  // TODO: To implement
  return BigInteger1024(0);
}


BigInteger2048 BigInteger2048::PowerMod(const BigInteger2048& a, const BigInteger2048& b) {
  // TODO: To implement
  return BigInteger2048(0);
}


std::vector<BigInteger2048> BigInteger2048::FromFile(const std::string &filename) {
  std::string line;
  std::ifstream file;
  file.open(filename);
  std::vector<BigInteger2048> result;

  while(std::getline(file, line))
  {
    result.emplace_back(BigInteger2048(line));
  }

  file.close();
  return result;
}
