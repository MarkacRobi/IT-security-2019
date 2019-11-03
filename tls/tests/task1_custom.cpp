#include "helpers.h"
#include <check.h>
#include "../counter.h"
#include "../hmac-sha2.h"
#include "../ascon128.h"
#include "../aes128gcm.h"
#include "../hkdf.h"

using util::operator""_k;
using util::operator""_x;

using namespace std;

// Include your custom testc ases here
// Write at LEAST 5 custom tests to verify your own implementation.
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
    std::vector<uint8_t> nonce_test_data_1 = {0x00, 0x10, 0x20, 0x30, 0x40, 0x50,
                                         0x60, 0x70, 0x80, 0x90, 0xa0, 0xb0};

    // FIPS 198a A.1
    const uint8_t key_fips_1[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                                  0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                                  0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
                                  0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
                                  0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
                                  0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f};

    const uint8_t text_fips_1[] = {'S', 'a', 'm', 'p', 'l', 'e', ' ', '#', '1'};

    const ascon128::key_storage key       = "ffffffffffffffffffffffffffffffff"_k;
    const std::vector<uint8_t> nonce_data = "000102030405060708090a0b0c0d0e0f"_x;

    const std::string plaintext_1 = "abcdefghijklmnoqrstuvwxyz0123456789ABCDEFGHIJKLMNOQRSTUVWXYZ";
    const std::string ad_1        = util::to_hex_string(plaintext_1);
    const std::vector<uint8_t> expected_1 =
            "7fc2b392364bcfe8fbc6417889e1c908beebf6e7378f96065df6616e10eb"
            "c798f1180f8d9188c8e4672558381fb18e4165aea627fcc9f18c8a8f54e7"
            "05615a12a7d5ef18b494b75eeca98879"_x;


}

// Sample to illustrate test:
//
// START_TEST(custom_1){
//   std::string part1 = "its";
//   std::string part2 = "_2019";
//   std::string to_check = part1 + part2;
//   ck_assert_str_eq(test_string.c_str(), to_check.c_str());
// }
// END_TEST

//check if counter is working correctly
START_TEST(nounce_test_1) {
    vector<incrementing_nonce> nonces;

    for(unsigned int i = 0; i < 1000; i++) {
        nonces.push_back(incrementing_nonce(nonce_test_data_1));
    }

    for(unsigned int i = 0; i < 1000; i++) {
        for(incrementing_nonce nonce: nonces) {
            ++nonce;
        }
    }
    incrementing_nonce first_nonce = nonces[0];

    for (incrementing_nonce nonce: nonces) {
        ck_assert_uint_eq(memcmp(first_nonce.nonce().data(), nonce.nonce().data(), first_nonce.nonce().size()), 0);
    }

}
END_TEST

//check reset
START_TEST(nounce_test_2) {
        vector<incrementing_nonce> incremented_nonces;
        vector<incrementing_nonce> original_nonces;

        for(unsigned int i = 0; i < 50; i++) {
            incremented_nonces.push_back(incrementing_nonce(nonce_test_data_1));
            original_nonces.push_back(incrementing_nonce(nonce_test_data_1));
        }

        for(unsigned int i = 0; i < 50; i++) {
            for(incrementing_nonce nonce: incremented_nonces) {
                ++nonce;
            }
        }
        incrementing_nonce first_nonce = incremented_nonces[0];

        for(unsigned int i = 0; i < 50; i++) {
            ck_assert_uint_eq(memcmp(incremented_nonces[i].nonce().data(), original_nonces[i].nonce().data(), first_nonce.nonce().size()), 0);
        }

    }
END_TEST

START_TEST(hmac_test1) {

    hmac_sha2 hmac(key_fips_1, sizeof(key_fips_1));

    hmac.update(text_fips_1, sizeof(text_fips_1));
    hmac_sha2::digest_storage tmp1 = hmac.digest();

    hmac.update(text_fips_1, sizeof(text_fips_1));
    hmac_sha2::digest_storage tmp2 = hmac.digest();

    ck_assert_str_eq((util::to_hex_string(tmp1)).c_str(), (util::to_hex_string(tmp2)).c_str());
}
END_TEST

START_TEST(ascon_test1) {
        incrementing_nonce nonce(nonce_data);
        ++nonce;

        ascon128 ascon(key);

        vector<uint8_t> ciphertext = {};
        vector<uint8_t> plaintext = {};
        bool res = ascon.encrypt(ciphertext, plaintext, nonce.nonce());

        ck_assert_uint_eq(res, true);
        ck_assert_uint_eq(ciphertext.size(), plaintext.size() + ascon128::additional_size );

        ++nonce;
        res = ascon.encrypt(ciphertext, plaintext, nonce.nonce());
        ck_assert_uint_eq(res, true);

        vector<uint8_t> decryptedText;
        res = ascon.decrypt(decryptedText, ciphertext, nonce.nonce());

        ck_assert_uint_eq(res, true);
        ck_assert_array_split_eq(plaintext, decryptedText);

}
END_TEST

START_TEST(aes_gcm_test1) {
        incrementing_nonce nonce(nonce_data);
        ++nonce;

        aes128gcm aesgcm(key);

        vector<uint8_t> plaintext = {};
        vector<uint8_t> ciphertext = {};
        bool res = aesgcm.encrypt(ciphertext, plaintext, nonce.nonce());
        ck_assert_uint_eq(res, true);
        ck_assert_uint_eq(ciphertext.size(), plaintext.size() + aes128gcm::additional_size);


        vector<uint8_t> plaintext_2;
        res = aesgcm.decrypt(plaintext_2, ciphertext, nonce.nonce());

        ck_assert_uint_eq(res, true);
        ck_assert_array_split_eq(plaintext, plaintext_2);


        ++nonce;
        vector<uint8_t> ciphertext2 = {};
        res = aesgcm.encrypt(ciphertext2, plaintext, nonce.nonce());
        ck_assert_uint_eq(res, true);

        vector<uint8_t> decryptedText;
        res = aesgcm.decrypt(decryptedText, ciphertext2, nonce.nonce());

        ck_assert_uint_eq(res, true);
        ck_assert_array_split_eq(plaintext, decryptedText);
}
END_TEST

START_TEST(hkdf_test1)
    {
        const vector<uint8_t> ikm          = ""_x;
        const vector<uint8_t> salt         = {};
        const vector<uint8_t> info         = {};
        const uint8_t l                    = 0;
        const vector<uint8_t> okm_expected = ""_x;

        hkdf kdf(salt, ikm);
        const auto okm = kdf.expand(info, l);
        ck_assert_array_split_eq(okm, okm_expected);
    }
END_TEST




int main(int argc, char** argv)
{
    Suite* suite = suite_create("Student Task 1 Tests");

    TCase* tCase_nonce = tcase_create("Nonce test");
    tcase_add_test(tCase_nonce, nounce_test_1);
    tcase_add_test(tCase_nonce, nounce_test_2);
    suite_add_tcase(suite, tCase_nonce);

    TCase* tCase_hmac = tcase_create("Hmac test");
    tcase_add_test(tCase_hmac, hmac_test1);
    suite_add_tcase(suite, tCase_hmac);

    TCase* tCase_ascon = tcase_create("Ascon test");
    tcase_add_test(tCase_ascon, ascon_test1);
    suite_add_tcase(suite, tCase_ascon);

    TCase* tCase_aes_gcm= tcase_create("Aes gcm test");
    tcase_add_test(tCase_aes_gcm, aes_gcm_test1);
    suite_add_tcase(suite, tCase_aes_gcm);

    TCase* tCase_hkdf= tcase_create("Hkdf test");
    tcase_add_test(tCase_hkdf, hkdf_test1);
    suite_add_tcase(suite, tCase_hkdf);
  // TCase* tcase = tcase_create("FIRST");
  // tcase_add_test(tcase, custom_1);
  // suite_add_tcase(suite, tcase);

    SRunner* suite_runner = srunner_create(suite);
    srunner_run(suite_runner, argc, argv);

    int number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    return !number_failed ? EXIT_SUCCESS : EXIT_FAILURE;
}
