#include "shamir.h"
#include <iostream>
#include <math.h>
#include "./utils.h"
#ifdef PROTOCOLS
#include "bigint_ref/BigInteger2048.h"
#else
#include "bigint/BigInteger2048.h"
#endif

shamir::shamir(uint8_t degree):degree(degree){
}

std::vector<BigInteger1024> getLagrange(const std::vector<BigInteger1024>& shares, uint8_t degree)
{
    std::vector<BigInteger1024> c;
    BigInteger1024 temp_c(0);

    for(int i=0; i<degree; i++)
    {
        BigInteger1024 n(1);
        BigInteger1024 d(1);

        for(int j=0; j<degree; j++)
        {
            if(i != j)
            {
                n = n * shares[j];
                d = d * (shares[j] - shares[i]);
            }
        }
        BigInteger1024 inverse = BigInteger1024::Inverse(d);
        //temp_c = n * inverse;
        c.push_back(n * inverse);
    }
    /*for(int i=0; i<c.size(); i++)
    {
        printf("BigInt is: %x\n", c.at(i).GetData()[i]);
    }*/
    return c;
}

BigInteger1024 shamir::reconstructSecret(const std::vector<BigInteger1024>& shares){

    std::vector<BigInteger1024> laGrange = getLagrange(shares, degree+1);
    printf("Pao\n");
    BigInteger1024 result(0);
    BigInteger1024 prvi_temp(0);
    BigInteger2048 temp(0);
    BigInteger2048 needed_value(0);
    BigInteger1024 r(0);
    BigInteger2048 c(0);
    for (auto i=0; i<degree+1; i++)
    {
        prvi_temp = prvi_temp + laGrange[i] * shares[i];
        /*BigInteger::MultiplyIntegers((word*)temp.GetData(), (word*)laGrange[i].GetData(),
                (word*)shares[i].GetData(), NUM_WORDS_1024);*/

        //memcpy((void*)needed_value.GetData(), (void*)temp.GetData(), NUM_BYTES_1024);

        //result = result + prvi_temp;
        /*BigInteger::AddIntegers((word*)c.GetData(), (word*)r.GetData(),(word*)needed_value.GetData(),
                (word*)(c.GetData()+NUM_BYTES_1024), NUM_WORDS_1024);

        mempcpy((void*)r.GetData(), (void*)c.GetData(), NUM_BYTES_1024);*/
    }
    printf("pao");
    return prvi_temp;
}
std::vector<BigInteger1024> shamir::generateShamirShares(){
    // TODO: generate the Shamir shares using the polynomial and indices which have to be created previously and return them
    return std::vector<BigInteger1024>();
}

std::vector<BigInteger1024> shamir::generateShamirPolynomial(const BigInteger1024& secret){
    // TODO: Generate a random Shamir polynomial for the given secret and store it.
    //       Return a vector with the coefficients starting with the coefficient of the constant term.
    return std::vector<BigInteger1024>();
}

std::vector<BigInteger1024> shamir::generateIndices(uint8_t numShares) {
    // TODO: Generate and store numShares random indices
    //       Return a vector containing them in the order in which they were generated.
    return std::vector<BigInteger1024>();
}

void shamir::setIndices(const std::vector<BigInteger1024> &indices) {
    shamir::indices = indices;
}
