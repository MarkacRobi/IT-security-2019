#include "shamir.h"
#include <iostream>
#include <math.h>
#include "./utils.h"
#include "bigint/BigInteger2048.h"

shamir::shamir(uint8_t degree):degree(degree){
}

BigInteger1024 shamir::reconstructSecret(const std::vector<BigInteger1024>& shares){
    // TODO: reconsruct the secret from the given shares, the degree is set in the constructor
    return BigInteger1024(0);
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

