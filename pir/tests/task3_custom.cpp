#include "../../utils/tests.h"
#include <check.h>
#include "../paillier.h"
#include "../bigint/BigInteger.h"

using namespace std;
// Include your custom testcases here
// Write at LEAST 10 custom tests to verify your own implementation.
//
// TODO:
//
// Each Test is encapsulated by:
//
// START_TEST(testname){
//   #testcode
// }
// END_TEST
//
// At the end of the test a verification statement such as ck_assert_int_eq(int, int),
// wich verifies that two ints are equal, should be called.
// For further information take a look at:
// https://libcheck.github.io/check/doc/doxygen/html/check_8h.html
//
// First create a Test Suite which holds all your testcases by calling suite_create(suite_name).
// Add Each test to the corresponding testcase by first creating a testcase object with
// tcase_create(tcase_name) and then call tcase_add_test(tcase, test_name) for each test you want
// to add to this testcase. Add the Tescases to the test suite by calling: suite_add_tcase(suite,
// tcase).
//
// Run your defined test suite and verify the outcome.
// You may look into some of our tests to get a feeling for the workflow.

namespace
{
    // include the constants you need for your test here
    // const std::string test_string = "its_2018";
    auto as = BigInteger1024::FromFile(SOURCE_DIR "data/as1024.txt");
    auto bs = BigInteger1024::FromFile(SOURCE_DIR "data/bs1024.txt");
    auto zero_1024 = bs[0];
    auto max_1024 = bs[2];
    const BigInteger1024 one_1024 = BigInteger1024(1);
}

//START_TEST(lcm){
//        ck_assert_bi_eq(paillier::lcm(as[0],bs[0]), zero_1024);
//    }
//END_TEST
//
//START_TEST(gcd){
//        BigInteger1024 a = BigInteger1024(10);
//        BigInteger1024 b = BigInteger1024(2);
//        ck_assert_bi_eq(paillier::gcd(a,b), BigInteger1024(10));
//    }
//END_TEST
//
//START_TEST(not_equals_empty){
//        printf("Testing not_equals_empty:\n");
//        ck_assert_int_eq(paillier::not_equal(BigInteger1024(),BigInteger1024()),false);
//    }
//END_TEST
//
//START_TEST(not_equals_zero){
//        printf("Testing not_equals_zero:\n");
//        ck_assert_int_eq(paillier::not_equal(BigInteger1024(0),BigInteger1024(0)), false);
//    }
//END_TEST

START_TEST(addition_empty_1024){
        auto res = BigInteger1024() + BigInteger1024();
        ck_assert_bi_eq(res, BigInteger1024());
    }
END_TEST

START_TEST(addition_zero_1024){
        auto res = BigInteger1024(0) + BigInteger1024(0);
        ck_assert_bi_eq(res, BigInteger1024(0));
    }
END_TEST

START_TEST(addition_empty_2048){
        auto res = BigInteger2048() + BigInteger2048();
        ck_assert_bi_eq(res, BigInteger2048());
    }
END_TEST

START_TEST(addition_zero_2048){
        auto res = BigInteger2048(0) + BigInteger2048(0);
        ck_assert_bi_eq(res, BigInteger2048(0));
    }
END_TEST

START_TEST(addition_zero_2048_1024){
        auto res = BigInteger2048(0) + BigInteger1024(0);
        ck_assert_bi_eq(res, BigInteger2048(0));
    }
END_TEST

START_TEST(addition_zero_1024_2048){
        auto res = BigInteger1024(0) + BigInteger2048(0);
        ck_assert_bi_eq(res, BigInteger2048(0));
    }
END_TEST

START_TEST(addition_empty_2048_1024){
        auto res = BigInteger2048() + BigInteger1024();
        ck_assert_bi_eq(res, BigInteger2048());
    }
END_TEST

START_TEST(addition_empty_1024_2048){
        auto res = BigInteger1024() + BigInteger2048();
        ck_assert_bi_eq(res, BigInteger2048());
    }
END_TEST

START_TEST(subtraction_empty_1024){
        auto res = BigInteger1024() - BigInteger1024();
        ck_assert_bi_eq(res, BigInteger1024());
    }
END_TEST

START_TEST(subtraction_zero_1024){
        auto res = BigInteger1024(0) - BigInteger1024(0);
        ck_assert_bi_eq(res, BigInteger1024(0));
    }
END_TEST

START_TEST(subtraction_empty_2048){
        auto res = BigInteger2048() - BigInteger2048();
        ck_assert_bi_eq(res, BigInteger2048());
    }
END_TEST

START_TEST(subtraction_zero_2048){
        auto res = BigInteger2048(0) - BigInteger2048(0);
        ck_assert_bi_eq(res, BigInteger2048(0));
    }
END_TEST

START_TEST(subtraction_zero_2048_1024){
        auto res = BigInteger2048(0) - BigInteger1024(0);
        ck_assert_bi_eq(res, BigInteger2048(0));
    }
END_TEST

START_TEST(subtraction_zero_1024_2048){
        auto res = BigInteger1024(0) - BigInteger2048(0);
        ck_assert_bi_eq(res, BigInteger2048(0));
    }
END_TEST

START_TEST(subtraction_empty_2048_1024){
        auto res = BigInteger2048() - BigInteger1024();
        ck_assert_bi_eq(res, BigInteger2048());
    }
END_TEST

START_TEST(subtraction_empty_1024_2048){
        auto res = BigInteger1024() - BigInteger2048();
        ck_assert_bi_eq(res, BigInteger2048());
    }
END_TEST

START_TEST(multiplication_empty_1024){
        auto res = BigInteger1024() * BigInteger1024();
        ck_assert_bi_eq(res, BigInteger1024());
    }
END_TEST

START_TEST(multiplication_zero_1024){
        auto res = BigInteger1024(0) * BigInteger1024(0);
        ck_assert_bi_eq(res, BigInteger1024(0));
    }
END_TEST

START_TEST(multiplication_empty_2048){
        auto res = BigInteger2048() * BigInteger2048();
        ck_assert_bi_eq(res, BigInteger2048());
    }
END_TEST

START_TEST(multiplication_zero_2048){
        auto res = BigInteger2048(0) * BigInteger2048(0);
        ck_assert_bi_eq(res, BigInteger2048(0));
    }
END_TEST

START_TEST(multiplication_zero_2048_1024){
        auto res = BigInteger2048(0) * BigInteger1024(0);
        ck_assert_bi_eq(res, BigInteger2048(0));
    }
END_TEST

START_TEST(multiplication_zero_1024_2048){
        auto res = BigInteger1024(0) * BigInteger2048(0);
        ck_assert_bi_eq(res, BigInteger2048(0));
    }
END_TEST

START_TEST(multiplication_empty_2048_1024){
        auto res = BigInteger2048() * BigInteger1024();
        ck_assert_bi_eq(res, BigInteger2048());
    }
END_TEST

START_TEST(multiplication_empty_1024_2048){
        auto res = BigInteger1024() * BigInteger2048();
        ck_assert_bi_eq(res, BigInteger2048());
    }
END_TEST

START_TEST(division_empty_2048_1024){
        auto res = BigInteger2048() / BigInteger1024();
        ck_assert_bi_eq(res, BigInteger1024());
    }
END_TEST

START_TEST(division_zero_2048_1024){
        auto res = BigInteger2048(0) / BigInteger1024(0);
        ck_assert_bi_eq(res, BigInteger1024(0));
    }
END_TEST

START_TEST(zero_greater_than_1024){
        const BigInteger1024 a = BigInteger1024(0);
        const BigInteger1024 b = BigInteger1024(0);
        ck_assert_int_eq(BigInteger::GreaterThan((word*)a.GetData(), (word*)b.GetData(), NUM_WORDS_1024, NUM_WORDS_1024), false);
    }
END_TEST
START_TEST(empty_greater_than_1024){
        const BigInteger1024 a = BigInteger1024();
        const BigInteger1024 b = BigInteger1024();
        ck_assert_int_eq(BigInteger::GreaterThan((word*)a.GetData(), (word*)b.GetData(), NUM_WORDS_1024, NUM_WORDS_1024), false);
    }
END_TEST

START_TEST(zero_greater_than_2048){
        const BigInteger2048 a = BigInteger2048(0);
        const BigInteger2048 b = BigInteger2048(0);
        ck_assert_int_eq(BigInteger::GreaterThan((word*)a.GetData(), (word*)b.GetData(), NUM_WORDS_2048, NUM_WORDS_2048), false);
    }
END_TEST

START_TEST(empty_greater_than_2048){
        const BigInteger2048 a = BigInteger2048();
        const BigInteger2048 b = BigInteger2048();
        ck_assert_int_eq(BigInteger::GreaterThan((word*)a.GetData(), (word*)b.GetData(), NUM_WORDS_2048, NUM_WORDS_2048), false);
    }
END_TEST


START_TEST(zero_smaller_than_1024){
        const BigInteger1024 a = BigInteger1024(0);
        const BigInteger1024 b = BigInteger1024(0);
        ck_assert_int_eq(BigInteger::SmallerThan((word*)a.GetData(), (word*)b.GetData(), NUM_WORDS_1024, NUM_WORDS_1024), false);
    }
END_TEST
START_TEST(empty_smaller_than_1024){
        const BigInteger1024 a = BigInteger1024();
        const BigInteger1024 b = BigInteger1024();
        ck_assert_int_eq(BigInteger::SmallerThan((word*)a.GetData(), (word*)b.GetData(), NUM_WORDS_1024, NUM_WORDS_1024), false);
    }
END_TEST

START_TEST(zero_smaller_than_2048){
        const BigInteger2048 a = BigInteger2048(0);
        const BigInteger2048 b = BigInteger2048(0);
        ck_assert_int_eq(BigInteger::SmallerThan((word*)a.GetData(), (word*)b.GetData(), NUM_WORDS_2048, NUM_WORDS_2048), false);
    }
END_TEST

START_TEST(empty_smaller_than_2048){
        const BigInteger2048 a = BigInteger2048();
        const BigInteger2048 b = BigInteger2048();
        ck_assert_int_eq(BigInteger::SmallerThan((word*)a.GetData(), (word*)b.GetData(), NUM_WORDS_2048, NUM_WORDS_2048), false);
    }
END_TEST


int main(int argc, char** argv)
{
    Suite* suite = suite_create("Student Task 3 Tests");

    //TCase* tcase_paillier = tcase_create("Paillier");
//   tcase_add_test(tcase_paillier, lcm);
  //  tcase_add_test(tcase_paillier, gcd);
  //  tcase_add_test(tcase_paillier, not_equals_zero);
   // tcase_add_test(tcase_paillier, not_equals_empty);
    //suite_add_tcase(suite, tcase_paillier);

    TCase* tcase_bigInt = tcase_create("BigInt");
    tcase_add_test(tcase_bigInt, addition_empty_1024);
    tcase_add_test(tcase_bigInt, addition_zero_1024);
    tcase_add_test(tcase_bigInt, addition_empty_2048);
    tcase_add_test(tcase_bigInt, addition_zero_2048);
    tcase_add_test(tcase_bigInt, addition_zero_2048_1024);
    tcase_add_test(tcase_bigInt, addition_zero_1024_2048);
    tcase_add_test(tcase_bigInt, addition_empty_2048_1024);
    tcase_add_test(tcase_bigInt, addition_empty_1024_2048);
    tcase_add_test(tcase_bigInt, subtraction_empty_1024);
    tcase_add_test(tcase_bigInt, subtraction_zero_1024);
    tcase_add_test(tcase_bigInt, subtraction_empty_2048);
    tcase_add_test(tcase_bigInt, subtraction_zero_2048);
    tcase_add_test(tcase_bigInt, subtraction_zero_2048_1024);
    tcase_add_test(tcase_bigInt, subtraction_zero_1024_2048);
    tcase_add_test(tcase_bigInt, subtraction_empty_2048_1024);
    tcase_add_test(tcase_bigInt, subtraction_empty_1024_2048);
    tcase_add_test(tcase_bigInt, multiplication_empty_1024);
    tcase_add_test(tcase_bigInt, multiplication_zero_1024);
    tcase_add_test(tcase_bigInt, multiplication_empty_2048);
    tcase_add_test(tcase_bigInt, multiplication_zero_2048);
    tcase_add_test(tcase_bigInt, multiplication_zero_2048_1024);
    tcase_add_test(tcase_bigInt, multiplication_zero_1024_2048);
    tcase_add_test(tcase_bigInt, multiplication_empty_2048_1024);
    tcase_add_test(tcase_bigInt, multiplication_empty_1024_2048);
    //tcase_add_test(tcase_bigInt, division_empty_2048_1024);
    //tcase_add_test(tcase_bigInt, division_zero_2048_1024);
    tcase_add_test(tcase_bigInt, zero_greater_than_1024);
    tcase_add_test(tcase_bigInt, empty_greater_than_1024);
    //tcase_add_test(tcase_bigInt, zero_greater_than_2048);
    //tcase_add_test(tcase_bigInt, empty_greater_than_2048);
    tcase_add_test(tcase_bigInt, zero_smaller_than_1024);
    tcase_add_test(tcase_bigInt, empty_smaller_than_1024);
    //tcase_add_test(tcase_bigInt, zero_smaller_than_2048);
    //tcase_add_test(tcase_bigInt, empty_smaller_than_2048);

    suite_add_tcase(suite, tcase_bigInt);



    SRunner* suite_runner = srunner_create(suite);
    srunner_run(suite_runner, argc, argv);

    int number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    return !number_failed ? EXIT_SUCCESS : EXIT_FAILURE;
}
