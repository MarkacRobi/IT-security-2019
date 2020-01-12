#include "../../utils/tests.h"
#include <check.h>
#include "../paillier.h"
#include "../bigint/BigInteger.h"
#include "../shamir.h"

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


    const BigInteger1024 secret_1_tc_1(42);

    const std::vector<BigInteger1024> given_indices = {
            BigInteger1024("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006c7d152f462e697132a005082c44b2423450112670159bce8e4a61f7ce79eaa1faac4cebb2ce5ff12228c6f254b93c56400a875ded50218c56d180ac50c0a202"),
            BigInteger1024("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005058d3856e3b33872ae6069eb5eaaac0532501f21379deaf8d5289e3f5cebca2fa63ae1121f84582ac2077ae844818ff9eba1f2eadacac7d741687cd659b496f"),
            BigInteger1024("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004070adffa0248c44fa9c4675eb9b5506c6660e9cadc3f38b504809c746139975589623a158eb93859cb4613435f26c9ea1e59db3059d78bd029d22326704a533"),
            BigInteger1024("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000007800b2c035a5ea82f0c1309426bd9fa34c51994295e0523294b3314afa8f5c2178e81714c51a61a686b4db0dd1c851d6ef406dc4e41a26887d1309952d5282a8"),
            BigInteger1024("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000678fb178b58bdbd6e4b2937b1dd75098dbf7579ecbce393dd0b69d8cfee73e40353adf350a383d0f963272a429d42900d10e00a2e94a1dbd4d5498b2f274268d"),
            BigInteger1024("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002abad5025e428fc46db56bad2923e9679e8eace0a2145e4a3b11d0e3ca160d56a595e62cb14782525bb378d70df8057b28d3a25ae09d6cb9a3add73fb92d03c8"),
            BigInteger1024("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000007f3c2f04b908f9f8b6ea5608332eeb4da75c5c12e192770a291fa7251b2a87430fce87845a3a19a719a69f9a5f7421028ea24a259f4c2373a5dbef3f209583ec"),
            BigInteger1024("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000030f15333f35066d671f0b76c8078ebc26f2be178e0508909fc655dc8f341c3b96e35d92f7009ad379193e610fbd7909783a0bef16f7740e0b2b94b1a943d7b6c"),
    };
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

START_TEST(share_single){
        uint8_t degree = 5;
        shamir test_share(degree);
        BigInteger1024 value(55);

        test_share.generateShamirPolynomial(value);
        test_share.setIndices(std::vector<BigInteger1024>(given_indices.begin(), given_indices.begin() + degree + 1));
        std::vector<BigInteger1024> shares = test_share.generateShamirShares();

        BigInteger1024 res_value = test_share.reconstructSecret(shares);

        ck_assert_bi_eq(value, res_value);

}END_TEST

START_TEST(share_two){
        uint8_t degree = 5;
        shamir test_two(degree);
        BigInteger1024 value(18);

        test_two.generateShamirPolynomial(value);
        test_two.setIndices(std::vector<BigInteger1024>(given_indices.begin(), given_indices.begin() + degree + 1));
        std::vector<BigInteger1024> shares = test_two.generateShamirShares();

        BigInteger1024 res_value = test_two.reconstructSecret(shares);

        ck_assert_bi_eq(value, res_value);

        shamir test_two__(degree);
        BigInteger1024 value__(17);

        test_two__.generateShamirPolynomial(value__);
        test_two__.setIndices(std::vector<BigInteger1024>(given_indices.begin(), given_indices.begin() + degree + 1));
        std::vector<BigInteger1024> shares__ = test_two__.generateShamirShares();

        BigInteger1024 res_value__ = test_two__.reconstructSecret(shares__);

        ck_assert_bi_eq(value__, res_value__);


}END_TEST



START_TEST(share_three){
        uint8_t degree = 5;
        shamir test_three(degree);
        BigInteger1024 value(33);

        test_three.generateShamirPolynomial(value);
        test_three.setIndices(std::vector<BigInteger1024>(given_indices.begin(), given_indices.begin() + degree + 1));
        std::vector<BigInteger1024> shares = test_three.generateShamirShares();

        BigInteger1024 res_value = test_three.reconstructSecret(shares);

        ck_assert_bi_eq(value, res_value);


        shamir test_three_(degree);
        BigInteger1024 value_(12);

        test_three_.generateShamirPolynomial(value_);
        test_three_.setIndices(std::vector<BigInteger1024>(given_indices.begin(), given_indices.begin() + degree + 1));
        std::vector<BigInteger1024> shares_ = test_three_.generateShamirShares();

        BigInteger1024 res_value_ = test_three_.reconstructSecret(shares_);

        ck_assert_bi_eq(value_, res_value_);



        shamir test_three__(degree);
        BigInteger1024 value__(20);

        test_three__.generateShamirPolynomial(value__);
        test_three__.setIndices(std::vector<BigInteger1024>(given_indices.begin(), given_indices.begin() + degree + 1));
        std::vector<BigInteger1024> shares__ = test_three.generateShamirShares();

        BigInteger1024 res_value__ = test_three.reconstructSecret(shares__);

        ck_assert_bi_eq(value, res_value__);

}END_TEST

START_TEST(polynomial_gen_1){
        uint8_t degree = 5;
        BigInteger1024 sec(25);
        shamir test_polynomial(degree);
        test_polynomial.generateIndices(degree);
        std::vector<BigInteger1024> pol1 = test_polynomial.generateShamirPolynomial(sec);
        std::vector<BigInteger1024> pol2 = test_polynomial.generateShamirPolynomial(sec);


        for(size_t i = 0; i < pol2.size(); i++){
            if(i == 0){
                ck_assert_bi_eq(pol1.at(i), pol2.at(i));
            }
            else
                ck_assert_bi_ne(pol1.at(i), pol2.at(i));
        }
    }END_TEST


START_TEST(polynomial_gen_2){
        uint8_t degree = 5;
        BigInteger1024 sec(66);
        shamir test_test(degree);
        test_test.generateIndices(degree);


        std::vector<BigInteger1024> pol1 = test_test.generateShamirPolynomial(sec);
        std::vector<BigInteger1024> pol2 = test_test.generateShamirPolynomial(sec);

        ck_assert_int_ne(pol1.size(), 0);
        ck_assert_int_ne(pol2.size(), 0);
        ck_assert_int_eq(pol1.size(), pol2.size());

        for(size_t i = 0; i < pol1.size(); i++){
            if(i != 0){
                ck_assert_bi_ne(pol1.at(i), pol2.at(i));
            }
            else
                ck_assert_bi_eq(pol1.at(i), pol2.at(i));
        }
    }END_TEST



int main(int argc, char** argv)
{
    Suite* suite = suite_create("Student Task 3 Tests");

    TCase* tcase_shamir = tcase_create("Shamir");
   tcase_add_test(tcase_shamir, share_single);
    tcase_add_test(tcase_shamir, share_two);
    tcase_add_test(tcase_shamir, share_three);
    tcase_add_test(tcase_shamir, polynomial_gen_1);
    tcase_add_test(tcase_shamir, polynomial_gen_2);


    suite_add_tcase(suite, tcase_shamir);

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

    //suite_add_tcase(suite, tcase_bigInt);



    SRunner* suite_runner = srunner_create(suite);
    srunner_run(suite_runner, argc, argv);

    int number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    return !number_failed ? EXIT_SUCCESS : EXIT_FAILURE;
}
