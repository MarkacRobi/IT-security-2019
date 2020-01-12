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


std::vector<BigInteger1024> getLagrange(std::vector<BigInteger1024>& shares)
{
    std::vector<BigInteger1024> c;

    for(int i=0; i< shares.size(); i++)
    {
        BigInteger1024 d(1);
        BigInteger1024 n(1);

        for(int j=0; j < shares.size(); j++)
        {
            if(i != j)
            {
                n = n * shares[j];
                d = d * (shares[j] - shares[i]);
            }
        }
        BigInteger1024 inverse = BigInteger1024::Inverse(d);
        BigInteger1024 result = inverse * n;
        c.push_back(result);
    }

    return c;
}

BigInteger1024 shamir::reconstructSecret(const std::vector<BigInteger1024>& shares)
{
    std::vector<BigInteger1024> laGrange = getLagrange(indices);
    BigInteger1024 result(0);

    for (auto i=0; i < shares.size(); i++)
    {
        result = result + laGrange[i] * shares[i];
    }

    return result;
}

std::vector<BigInteger1024> shamir::generateShamirShares(){
    // TODO: generate the Shamir shares using the polynomial and indices which have to be created previously and return them
    return std::vector<BigInteger1024>();
}

std::vector<BigInteger1024> shamir::generateShamirPolynomial(const BigInteger1024& secret){
    // TODO: Generate a random Shamir polynomial for the given secret and store it.
    //       Return a vector with the coefficients starting with the coefficient of the constant term.

    //if(polynomial.size() == 0)
    std::vector<BigInteger1024>polynomial_temp;

    for(int i = 0; i < degree + 1; i++)
    {
        if(i == 0)
        {
            polynomial_temp.push_back(secret);
            polynomial.push_back(secret);
        }
        else
        {
            BigInteger1024 temp;
            temp.Randomize();

            polynomial.push_back(temp);
            polynomial_temp.push_back(temp);
        }
    }
    return polynomial_temp;
}

std::vector<BigInteger1024> shamir::generateIndices(uint8_t numShares) {
    // TODO: Generate and store numShares random indices
    //       Return a vector containing them in the order in which they were generated.
    for(int i = 0; i < numShares; i++)
    {
        BigInteger1024 random;
        random.Randomize();
        indices.push_back(random);
    }
    setIndices(indices);
    return indices;
}

void shamir::setIndices(const std::vector<BigInteger1024> &indices) {
    shamir::indices = indices;
}
