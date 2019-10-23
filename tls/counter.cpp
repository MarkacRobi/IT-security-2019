#include "counter.h"
#include "endian.h"

#define  HEX1 0xff00000000000000
#define  HEX2 0xff000000000000
#define  HEX3 0xff0000000000
#define  HEX4 0xff00000000
#define  HEX5 0xff000000
#define  HEX6 0xff0000
#define  HEX7 0xff00
#define  HEX8 0xff
incrementing_nonce::incrementing_nonce(const std::vector<uint8_t>& bytes) :  counter(0)
{
    unsigned long it = 0;
    auto size = bytes.size();
    while(it  < size)
    {

            randomness.push_back(bytes[it]);
            it++;
    }

}

incrementing_nonce& incrementing_nonce::operator++()
{counter++;
  return *this;
}

void incrementing_nonce::reset(const std::vector<uint8_t>& bytes)
{
    randomness.clear();

    counter = 0;
    unsigned long it = 0;
    auto size = bytes.size();
    while(it  < size)
    {

        randomness.push_back(bytes[it]);
        it++;
    }


  /// \todo reset the nonce
}

const std::vector<uint8_t>& incrementing_nonce::nonce()
{
    randomness_copy.clear();
    unsigned long it = 0;
    auto size = randomness.size() - 8;
    while(it  < size) {

        randomness_copy.push_back(randomness[it]);


        it++;
    }


    randomness_copy.push_back(randomness[randomness.size() - 8] ^
    (counter & HEX1));

    randomness_copy.push_back(randomness[randomness.size() - 7] ^
    (counter & HEX2));

    randomness_copy.push_back(randomness[randomness.size() - 6] ^
    (counter & HEX3));

    randomness_copy.push_back(randomness[randomness.size() - 5] ^
    (counter & HEX4));


    randomness_copy.push_back(randomness[randomness.size() - 4] ^
    (counter & HEX5));
    randomness_copy.push_back(randomness[randomness.size() - 3] ^
    (counter & HEX6));

    randomness_copy.push_back(randomness[randomness.size() - 2] ^
    (counter & HEX7));
    randomness_copy.push_back(randomness[randomness.size() - 1] ^
    (counter & HEX8));

    return_vector.clear();
    unsigned long iter = 0;
    auto size2 = randomness_copy.size() ;
    while(iter  < size2) {
        return_vector.push_back(randomness_copy[iter]);
        iter++;
    }


    return return_vector;

}
