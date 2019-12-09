#ifndef PAILLIER_H
#define PAILLIER_H

#include "bigint/BigInteger1024.h"
#include "bigint/BigInteger2048.h"


class paillier{
public:
    paillier();

    // Constants
    static const BigInteger1024 one;
    static const BigInteger1024 zero;
    static const BigInteger1024 p;
    static const BigInteger1024 q;
    static const BigInteger1024 m;

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
