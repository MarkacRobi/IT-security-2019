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
#include "BigInteger2048.h"
#define mu 0x179bd99a80d40c4a0164cefb86800433593888f1be00ea82365fdf2c5f3bbb8a387e0b507fc61db6ff99597781dafadc639d20eb82e6cf5d0aae5505b252e51c48458817e583e7a841836ad6bcc93a11ede1731e723df2e3bf18482b8959e54461c7cb693ac24b7097f524e1e068949020ec66df8400ceae571b7ccb64e562db6L

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

 void print_word(word* c, uint32 num_words_operands) {
    printf("Printing word c: \n");
     for (size_t i = 0; i <= num_words_operands - 1; i++) {
//         printf("%u|", (unsigned int)c[i]);
            std::cout << std::hex << static_cast<int>(c[i]);
     }
//        stream  << std::hex << c;
//        std::cout << stream.str();
        printf("\n");

}



void BigInteger::AddIntegers(word* c, word* a, word* b, word* carry, uint32 num_words_operands) {
    // TODO: To implement

    uint32 temp_result = 0;

    temp_result = a[0] + b[0];
    c[0] = a[0] + b[0];
    *carry = temp_result >> 16;

    //printf("-<-<<-<-<-<-<--<-<-%x\n", temp_result);

    for (size_t i = 1; i <= num_words_operands - 1; i++) {

        temp_result = a[i] + b[i] + *carry;
        c[i] = a[i] + b[i] + *carry;
        *carry = temp_result >> 16;
    }


}


void BigInteger::SubtractIntegers(word* c, word* a, word* b, word* borrow, uint32 num_words_operands) {
  // TODO: To implement

    int borrow_ = 0;
    int temp_result = a[0] - b[0];
    if(temp_result < 0)
    {
        //printf("negativno je\n");
        temp_result += 0x10000;
        borrow_ = 1;

    }
    //printf("---------------->%x\n", temp_result);
    //printf("ispod if\n");
    c[0] = temp_result;

    for (size_t i = 1; i <= num_words_operands - 1; i++) {

        temp_result = a[i] - b[i] - borrow_;

        if(temp_result < 0)
        {
            temp_result += 0x10000;
            borrow_ = 1;

        } else{
            borrow_ = 0;
        }

        c[i] = temp_result;

    }

    *borrow = borrow_;
//    printf("borrwo is %x\n", *borrow);

}

void BigInteger::MultiplyIntegers(word* c, word* a, word* b, uint32 num_words_operands) {
    // TODO: To implement
    uint32 carry = 0; // u
    unsigned int i = 0;
    unsigned int j = 0;
    uint64 current_number = 0;

    for(i = 0; i <= num_words_operands - 1; i++)
    {
        c[i] = 0x0;
    }
    for(i = 0; i <= num_words_operands - 1; i++)
    {
        carry = 0;
        for(j = 0; j <= num_words_operands - 1; j++)
        {
            current_number = c[i+j] + (a[i] * b[j]) + carry;
            if (current_number > 0xFFFF)
            {
                carry = current_number >> 16;
                current_number -= 0x10000;

            }
            else
            {
                carry = 0;
            }

            c[i+j] = current_number;
            printf("temp iznad res na index %d je: %x\n", i+j,  c[j + i]);
        }
        c[num_words_operands + i] = carry;
        printf("temp iznad res na index %d je: %x\n", i + num_words_operands,  c[i + num_words_operands]);
    }

}



void BigInteger::KaratsubaOfman(word* c, word* a, word* b, uint32 num_words_operands) {
  // TODO: To implement
}


bool BigInteger::SmallerThan(word* a, word* b, uint32 num_words_a, uint32 num_words_b) {
  // TODO: To implement
    int i;
    if(num_words_a == num_words_b)
    {
        i = num_words_a -1;
    }
    else if(num_words_a < num_words_b)
    {
        i = num_words_b -1;
    }
    else if(num_words_a > num_words_b)
    {
        i = num_words_a -1;
    }

    for(; i >= 0; i--)
    {
        //printf("a je %x\n", a[i]);
        //printf("b je %x\n", b[i]);
        if(a[i] < b[i])
        {
            //printf("vece je\n");
            return true;
        }
        else if(a[i] > b[i])
        {
            //printf("manje je\n");
            return false;
        }

    }
  return false;
}

bool BigInteger::GreaterThan(word* a, word* b, uint32 num_words_a, uint32 num_words_b) {
  // TODO: To implement
  //printf("%x\n", a[0]);

//
//            printf("a is: ");
//    for(int i = 0; i < 128; i++)
//    {
//        printf("%x", a[i]);
//    }
//    printf("\n");
//
//    printf("b is: ");
//    for(int i = 0; i < 128; i++)
//    {
//        printf("%x", b[i]);
//    }
//    printf("\n");




    int i;
  if(num_words_a == num_words_b)
  {
      i = num_words_a -1;
  }
  else if(num_words_a < num_words_b)
  {
      i = num_words_b -1;
  }
  else if(num_words_a > num_words_b)
  {
      i = num_words_a -1;
  }

  for(; i >= 0; i--)
  {
      //printf("a je %x\n", a[i]);
      //printf("b je %x\n", b[i]);
      if(a[i] > b[i])
      {
          //printf("vece je\n");
          return true;
      }
      else if(a[i] < b[i])
      {
          //printf("manje je\n");
          return false;
      }

  }

  return false;
}
