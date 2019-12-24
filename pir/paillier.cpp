#include "paillier.h"

paillier::paillier(){
    // \TODO Initialize g with a random number and initialize the private keys correctly
    g = BigInteger2048();
    lambda = BigInteger1024();
    mue = BigInteger1024();
}


const BigInteger1024 paillier::one = BigInteger1024(1);
const BigInteger1024 paillier::zero = BigInteger1024(0);


const std::string s("fff6e927f09e8acd909fe2a501799329f9b37874ded2ace58eb743fed2b375805c9ca946d92a92c727a8638fbd14e0e2027465457f3ea82a9e3659174711f177");
const std::string t("ad84ce6895f481e4cd8431fe63a7efcab27d44b8a42b6292e2f0dfff5d1b389aabb3571a24877d32e34b09066f3bc987e70e0b915fd0a0051afd07f201a66ed7");
const std::string u("ad7ea541780f40507ad3b837bc3049331aaee8610dcd93472a182a206c1b166fa9bd2b39b3cd083e6b7bd175b22b16592bd21a518028b0ee6f3ca01d26219cf8e00de5741dc2d02335e7b392e86e0c663eb3abf93d157b436f3cb98d2fd296e81773ac484b960201de442a9cdb49b8b53aec4129691f5ae47b777e286ffcecf1");
const BigInteger1024 paillier::p = BigInteger1024(std::string(128, '0').append(s));
const BigInteger1024 paillier::q = BigInteger1024(std::string(128, '0').append(t));
const BigInteger1024 paillier::m = BigInteger1024(u);



BigInteger1024 paillier::decrypt(const BigInteger2048 &ciphertext)
{
  // \TODO decrypt the given ciphertext using the private keys mue and lmabda and return the resulting plaintext
  return BigInteger1024();
}

BigInteger2048 paillier::encrypt(const BigInteger1024 &plaintext)
{
    // \TODO Encrypt the plaintext using the g calculated during the key setup and a randomly selected rho
    // \TODO Make sure to check the restrictions on the involved values
    return BigInteger2048();
}
