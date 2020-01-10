#ifndef SHAMIR_H
#define SHAMIR_H

#ifdef PROTOCOLS
#include "bigint_ref/BigInteger1024.h"
#else
#include "bigint/BigInteger1024.h"
#endif
#include <vector>
#include <cstdint>
#include <cstddef>
#include <string>
#include "primes.h"

class shamir
{
    private:
    uint8_t degree;
    std::vector<BigInteger1024> indices;
    public:
        shamir(uint8_t degree);
        std::vector<BigInteger1024> generateShamirPolynomial(const BigInteger1024& secret);
        std::vector<BigInteger1024> generateShamirShares();
        BigInteger1024 reconstructSecret(const std::vector<BigInteger1024>& shares);
        std::vector<BigInteger1024> generateIndices(uint8_t numShares);

        void setIndices(const std::vector<BigInteger1024> &indices);
};
#endif
