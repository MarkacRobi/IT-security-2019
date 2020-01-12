#include "paillier.h"
#include "bigint/BigInteger.h"
#include "primes.h"

paillier::paillier(){
    // \TODO Initialize g with a random number
    g = BigInteger2048();
    g.Randomize();
    // TODO initialize the private keys correctly
    lambda = lcm(p - one, q - one);
    // if using p,q of equivalent length, this is a simpler variant from WIKIPEDIA
    // TODO: check if it is enough, otherwise use harder variant below
    mue = BigInteger1024::Inverse(lambda);
    // harder variant for case when p an q are not equal -> mue = (L(g^lambda mod m^2))^-1 mod m
//    const BigInteger1024 g_on_lambda_mod_m2 = BigInteger2048::PowerMod(g, BigInteger2048(lambda)) / m;
//    mue = BigInteger1024::Inverse(L(g_on_lambda_mod_m2));
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
  // formula P = L(ciphertext^lambda (mod m^2)) * mue (mod m)
  // calculate C to power of lambda, which is moduled by m, and then divide again by m to get moduled by m^2 ???
  const BigInteger1024 C_on_lambda_mod_m2 = BigInteger2048::PowerMod(ciphertext, BigInteger2048(lambda)) / m;

  return L(C_on_lambda_mod_m2) * mue;
}

BigInteger2048 paillier::encrypt(const BigInteger1024 &plaintext)
{
    // \TODO Encrypt the plaintext using the g calculated during the key setup and a randomly selected rho
    // \TODO Make sure to check the restrictions on the involved values
    return BigInteger2048();
}

bool paillier::not_equal(const BigInteger1024& a, const BigInteger1024& b) {
    if(BigInteger::GreaterThan((word*)a.GetData(), (word*)b.GetData(), NUM_WORDS_1024, NUM_WORDS_1024)) {
        printf("a is greater than b\n");
        return true;
    }
    if(BigInteger::SmallerThan((word*)a.GetData(), (word*)b.GetData(), NUM_WORDS_1024, NUM_WORDS_1024)) {
        printf("a is smaller than b\n");
        return true;
    }
    printf("a is not greater or smaller than b -> returning false\n");
    return false;
}
// L(u) = round_down((u - 1) / m ) page 35
BigInteger1024 paillier::L(const BigInteger1024& u_) {
    return BigInteger2048(u_ - one) / m;
}
// lcm we need to calculate lambda = lcm(p - 1, q - 1)
BigInteger1024 paillier::lcm(const BigInteger1024& p_minus_1, const BigInteger1024& q_minus_1) {
    return BigInteger2048(p_minus_1 * q_minus_1) / gcd(p_minus_1, q_minus_1);
}
// Euclidean algorithm, page 36 - Algorithm 9 TODO: check if making new BigInt1024 objects makes sense
BigInteger1024 paillier::gcd(const BigInteger1024& a, const BigInteger1024& b) {
    BigInteger1024 a_ = BigInteger1024(a);
    BigInteger1024 b_ = BigInteger1024(b);

    while(not_equal(a_,b_)) {
        if(BigInteger::GreaterThan((word*)a_.GetData(), (word*)b_.GetData(), NUM_WORDS_1024, NUM_BYTES_1024)) {
            a_ = a_ - b_;
        } else {
            b_ = b_ - a_;
        }
    }

    BigInteger1024 r = BigInteger1024(a_);
    return r;
}

