//
// Created by Edi Muskardin on 15/11/2019.
//

#ifndef ITS2018_PRIMES_H
#define ITS2018_PRIMES_H

#ifdef PROTOCOLS
#include "bigint_ref/BigInteger1024.h"
#else
#include "bigint/BigInteger1024.h"
#endif
#include "string"

namespace primes{
    const std::string s("fff6e927f09e8acd909fe2a501799329f9b37874ded2ace58eb743fed2b375805c9ca946d92a92c727a8638fbd14e0e2027465457f3ea82a9e3659174711f177");
    const std::string t("ad84ce6895f481e4cd8431fe63a7efcab27d44b8a42b6292e2f0dfff5d1b389aabb3571a24877d32e34b09066f3bc987e70e0b915fd0a0051afd07f201a66ed7");
    const std::string u("ad7ea541780f40507ad3b837bc3049331aaee8610dcd93472a182a206c1b166fa9bd2b39b3cd083e6b7bd175b22b16592bd21a518028b0ee6f3ca01d26219cf8e00de5741dc2d02335e7b392e86e0c663eb3abf93d157b436f3cb98d2fd296e81773ac484b960201de442a9cdb49b8b53aec4129691f5ae47b777e286ffcecf1");
    const BigInteger1024 p = BigInteger1024(std::string(128, '0').append(s));
    const BigInteger1024 q = BigInteger1024(std::string(128, '0').append(t));
    const BigInteger1024 m = BigInteger1024(u);
}

#endif //ITS2018_PRIMES_H
