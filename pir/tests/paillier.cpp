#include "../paillier.h"
#include "../../utils/tests.h"
#include "../random.h"
#include <check.h>
#include <cstdlib>

namespace{
}

START_TEST(key_creation)
{
    uchar seed[18] = "CryptoIsQuiteNice";
    Random::Init(seed);
    auto p = paillier();
    auto ref = BigInteger1024::FromFile(SOURCE_DIR "data/test_key_generation.txt");
    ck_assert_bi_eq(p.getLambda(), ref[0]);
    ck_assert_bi_eq(p.getMue(), ref[1]);
}
END_TEST

START_TEST(single_encrypt)
{
    uchar seed[17] = "LetsEncryptStuff";
    Random::Init(seed);
    auto p = paillier();
    auto plain = BigInteger1024(123454321);
    auto C = p.encrypt(plain);
    auto ref = BigInteger2048::FromFile(SOURCE_DIR "data/test_single_encrypt.txt");
    ck_assert_bi_eq(C, ref[0]);
}
END_TEST

START_TEST(random_encrypt)
{
    auto p = paillier();
    auto plain = BigInteger1024();
    for(auto i = 0; i < 10; i++)
    {
        plain.Randomize();
        auto c = p.encrypt(plain);
        auto d = p.decrypt(c);
        ck_assert_bi_eq(d, plain);
    }
}
END_TEST

START_TEST(random_element_consistency)
{
    auto p = paillier();
    auto plain = BigInteger1024(123454321);
    for(auto i = 0; i < 5; i++)
    {
        plain.Randomize();
        auto c1 = p.encrypt(plain);
        auto c2 = p.encrypt(plain);
        auto d1 = p.decrypt(c1);
        auto d2 = p.decrypt(c2);
        ck_assert_bi_eq(d1, d2);
    }
}
END_TEST


START_TEST(homomorphic_addition1)
{
    auto p = paillier();
    const auto g = BigInteger1024(29);
    const auto rho1 = BigInteger1024(29);
    const auto rho2 = BigInteger1024(17);
    BigInteger1024 p1 = BigInteger1024();
    BigInteger1024 p2 = BigInteger1024();
    p1.Randomize();
    p2.Randomize();
    auto c1 = p.encrypt(p1);
    auto c2 = p.encrypt(p2);
    ck_assert_bi_eq(p.decrypt(c1 * c2), p1 + p2);
}
END_TEST

START_TEST(homomorphic_addition2)
{
    auto p = paillier();
    BigInteger1024 p1 = BigInteger1024();
    BigInteger1024 p2 = BigInteger1024();
    p1.Randomize();
    p2.Randomize();
    auto c1 = p.encrypt(p1);
    ck_assert_bi_eq(p.decrypt(c1 * BigInteger2048::PowerMod(p.getg(), p2)), p1 + p2);

  }
END_TEST

START_TEST(homomorphic_multiplication1)
{
    auto p = paillier();
    BigInteger1024 p1 = BigInteger1024();
    BigInteger1024 p2 = BigInteger1024();
    p1.Randomize();
    p2.Randomize();
    auto c1 = p.encrypt(p1);
    ck_assert_bi_eq(p.decrypt(BigInteger2048::PowerMod(c1, p2)), p1 * p2);
}
END_TEST

START_TEST(homomorphic_multiplication2)
{
    auto p = paillier();
    BigInteger1024 p1 = BigInteger1024();
    BigInteger1024 p2 = BigInteger1024();
    p1.Randomize();
    p2.Randomize();
    auto c2 = p.encrypt(p2);
    ck_assert_bi_eq(p.decrypt(BigInteger2048::PowerMod(c2, p1)), p1 * p2);
}
END_TEST

int main(int argc, char** argv)
{
   Suite* suite = suite_create("Paillier");

    TCase* tcase = tcase_create("BasicEncryption");
    tcase_add_test(tcase, key_creation);
    tcase_add_test(tcase, single_encrypt);
    tcase_add_test(tcase, random_element_consistency);
    tcase_add_test(tcase, random_encrypt);
    suite_add_tcase(suite, tcase);


    tcase = tcase_create("SystemProperties");
    tcase_add_test(tcase, homomorphic_addition1);
    tcase_add_test(tcase, homomorphic_addition2);
    tcase_add_test(tcase, homomorphic_multiplication1);
    tcase_add_test(tcase, homomorphic_multiplication2);
    suite_add_tcase(suite, tcase);

    SRunner* suite_runner = srunner_create(suite);
    srunner_run(suite_runner, argc, argv);
    int number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    return !number_failed ? EXIT_SUCCESS : EXIT_FAILURE;
}
