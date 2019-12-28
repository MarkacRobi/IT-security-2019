// Some functions here (e.g., the additions with the if clauses) are not constant-time. However, they can easily be made constant-time with small modifications.
// Compile with g++ -std=c++0x -g -O2 -march=native -mtune=native <...> -lcrypto

#include "BigInteger.h"
#include <math.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>
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


int extractDigit(word word_, int index){
    if(index == 0)
        return word_  % 16;

    if(index == 1)
        return (word_ / 16) % 16;

    if(index == 2)
        return (word_ / 16 / 16)  % 16;

    if(index == 3)
        return (word_ / 16 / 16 / 16)  % 16;

    //should never happen
    return 0;
}

bool checkIfIsThreeDigit(int num)
{
    return num > 0xFF && num <= 0xFFF;
}

int changeOrder(int num)
{
    printf("num is %x\n", num);
    bool flag = false;
    std::string temp_;
    if(checkIfIsThreeDigit(num))
    {
        flag = true;

     }
    int rem;

    std::string result = "";
    std::vector<int> temp;
    std::string decimal_place = "TEST";
    for (int i = 0; i < 4; i++)
    {
        rem = num % 16;
        num = num /16;

        temp.push_back(rem);
    }

    doubleword result__ = temp[0] | temp[1] << 4;
            doubleword result2 = (temp[2]) | (temp[3] <<4);
            doubleword result3 = (result__ << 8) | result2;

    if(!flag)
        return result3;
    else
    {

        doubleword res_t = temp[0] | temp[1] << 4;
        doubleword result_final = (res_t << 4) | temp[2];
        return result_final;
    }


 }



void BigInteger::AddIntegers(word* c, word* a, word* b, word* carry, uint32 num_words_operands) {
    // TODO: To implement

    int temp_result = 0;
    *carry = 0;

    temp_result = a[0] + b[0];
    if(temp_result > 65535)
    {
        *carry = 1;
        temp_result -= 65535;
    }
    c[0] = temp_result;

    for (size_t i = 1; i <= num_words_operands - 1; i++) {

        temp_result = a[i] + b[i] + *carry;

        if(temp_result > 0xFFFF)
        {
            *carry = 1;
            temp_result -= 65536;
        } else{
            *carry = 0;
        }

        c[i] = temp_result;

    }

    /*if(*carry== 1 || c >= (word*)primes::m.GetData())
    {
        *c = c - (word*)primes::m.GetData();
    }*/

}

void BigInteger::SubtractIntegers(word* c, word* a, word* b, word* borrow, uint32 num_words_operands) {
  // TODO: To implement

    //int jedan  = 0x0F2D;
    //int dva = 0xA23C;
    //printf("--->%x\n", jedan - dva);


    *borrow = 0;

    int temp_result = a[0] - b[0];;
    if(temp_result < 0)
    {
        printf("negativno je\n");
        temp_result += 0x10000;
        *borrow = 1;

    }
    //printf("---------------->%x\n", temp_result);

    c[0] = temp_result;

    for (size_t i = 1; i <= num_words_operands - 1; i++) {

        temp_result = a[i] - b[i] - *borrow;

        if(temp_result < 0)
        {
            temp_result += 0x10000;
            *borrow = 1;

        } else{
            *borrow = 0;
        }

        c[i] = temp_result;

    }

    printf("primes is: ");
    for(size_t i = 0; i < 128; i++)
    {
        printf("%x", (word*)primes::m.GetData()[i]);
    }
    printf("\n");

//    if(*borrow == 1)
//        AddIntegers(c, c, (word*)primes::m.GetData(), borrow, num_words_operands);

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
