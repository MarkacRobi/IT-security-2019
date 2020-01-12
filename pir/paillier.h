#ifndef PAILLIER_H
#define PAILLIER_H

#ifdef PROTOCOLS
#include "bigint_ref/BigInteger1024.h"
#include "bigint_ref/BigInteger2048.h"
#else
#include "bigint/BigInteger1024.h"
#include "bigint/BigInteger2048.h"
#endif

class paillier{
public:
    paillier();

    // Constants
    static const BigInteger1024 one;
    static const BigInteger1024 zero;
    static const BigInteger1024 p;
    static const BigInteger1024 q;
    static const BigInteger1024 m;

    // least common multiple
    static BigInteger1024 lcm(const BigInteger1024& p_minus_one, const BigInteger1024& q_minus_1);
    // greatest common divisor
    static BigInteger1024 gcd(const BigInteger1024& a, const BigInteger1024& b);
    // L(u) = floor((u - 1) / m)
    static BigInteger1024 L(const BigInteger1024& u);
    //  util function to check if two bigInts are not equal
    static bool not_equal(const BigInteger1024& a, const BigInteger1024& b);

    // Encrypt & Decrypt
    BigInteger2048 encrypt(const BigInteger1024 &plaintext);
    BigInteger1024 decrypt(const BigInteger2048 &ciphertext);

    // Needed for key creation testing
    BigInteger1024 getMue(){return mue;};
    BigInteger1024 getLambda(){return lambda;};
    BigInteger2048 getg(){return g;};
private:
    // Private keys and g
    // Initialize these values in the constructor
    BigInteger1024 mue, lambda;
    BigInteger2048 g;
};

#endif
