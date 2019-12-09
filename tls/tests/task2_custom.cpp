#include "../../utils/tests.h"
#include "../../utils/utils.h"
#include <boost/asio.hpp>
#include "../tls-record-layer.h"
#include <string>
#include <check.h>
#include "../ecdh.h"
#include "../secrets.h"
#include "../aes128gcm.h"
#include "../ascon128.h"
#include "../endian.h"
#include "../hkdf.h"
#include "../tls-aesgcm.h"
#include "../tls-ascon.h"

using namespace std;
using boost::asio::ip::tcp;
using util::operator""_x;

// Include your custom testc ases here
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
    const vector<uint8_t> some_messages =
            "010000c5030359f38fc78fd64ef8e8aec2c486ef2247b7c2416ca9f421aff5505ef505d7698b01000004ff011301"
            "010000970033004700450017004104c0d645154c89c3b3725dfc4884a35d25406e100bff63392e7a406f2624d220"
            "b9c9776a3a81fd538ae41b38b0616de06b937cd9b447960ef5c137fbff03a51a1a002b0003020304002d00020101"
            "000a00040002001700290033000e00086964656e7469747900000000002120cbefc811e3e2fdae06abf5c8adcd92"
            "b2db375fc11828a9f811c6529f0b13c43c0200007e030359f3902e975920bd7bb7544e495129953837631e598b63"
            "a20f91c84b17c960120100130100005500330045001700410499484aed20b35a570d06af3f84194eee23a1af4318"
            "00c52e0c9356f123ffdc0588ae598e18bd9ff892fd3251fe4bea0a5512ac7da7fcb474656f59ab3ad9684e002b00"
            "020304002900020000"_x;
    const vector<uint8_t> psk = "abab"_x;
    const vector<uint8_t> dhe =
            "86e94ce15c700d63c13433f73839bdfb23617c631a149e395aa646ac762da770"_x;

    auto expected_client_finished_key =
            "18a116ceb62b832ba4217f87e2b0fd972a854f0391cc352ab3a315101f006edd"_x;
    auto expected_server_finished_key =
            "d988598717658aba6c0bdafdbc45c83a73739d67f6dc9b7c55eb9ca0ff67a88a"_x;
    const vector<uint8_t> expected_derived_key_empty =
            "d792d9917b9a2d2c2bb25799aebd83e224337d71d7eb1b8701b8f04954985298"_x;
    const vector<uint8_t> expected_early_secret_empty =
            "41ba791c6035d6f3953c0858645f4bd866af2c1c33b40db93dcb6d1b26140083"_x;

    vector<uint8_t> k_lab = {'t', 'e', 's', 't'};
    vector<uint8_t> n_lab = {'t', 'e', 's', 't'};
    vector<uint8_t> salt =  {61,88,61,246,231,134,45,111,80,227,6,208,10,253,207,32};
    vector<uint8_t> ikm = {6,237,231,117,246,84,45,99,216,238,227,215,67,10,254,207};

    vector<vector<uint8_t>> empty_plaintext_cipher = {{74,  50,  119, 165, 103, 25,  119, 19,  95, 142, 214, 108, 5,   149, 208, 200, 34},
                                                      {3,   163, 179, 194, 209, 143, 215, 202, 7,  77,  149, 117, 75,  64,  86,  166, 230},
                                                      {122, 11,  190, 201, 254, 129, 78,  251, 74, 100, 255, 211, 202, 46,  126, 6,   145},
                                                      {86,  199, 213, 163, 65,  206, 207, 150, 89, 254, 199, 134, 22,  7,   149, 25,  211},
                                                      {12,  36,  1,   14,  67,  185, 245, 187, 146, 135, 42,  249, 5,   143, 149, 182, 247},
                                                      {225,87,246,131,217,199,252,180,231,122,44,60,93,103,130,202,211},
                                                      {145,57,46,56,156,71,101,13,249,197,9,241,252,125,115,116,20},
                                                      {2,79,66,240,210,221,157,177,89,227,59,46,173,191,251,151,236},
                                                      {212,151,244,26,213,20,236,125,64,199,73,16,134,249,245,251,232},
                                                      {69,10,130,224,153,83,165,43,108,3,53,18,140,85,67,108,248},
                                                      {51,203,185,96,124,209,117,34,102,23,254,89,136,212,253,211,132},
                                                      {30,231,253,165,38,233,52,210,171,173,33,209,170,19,127,228,58},
                                                      {73,77,89,12,157,132,135,241,11,209,148,236,251,228,223,126,0},
                                                      {3,48,170,118,250,93,54,239,60,2,216,187,69,83,33,133,87},
                                                      {187,121,170,236,246,95,83,177,174,13,15,237,194,196,92,15,64}};

    vector<vector<uint8_t>> zero_plaintext_cipher = {{199, 57,  109, 148, 186, 115, 145, 236, 187, 187, 74,  71,  3,   197, 255, 208, 17,  96},
                                                     {54,  237, 79,  153, 112, 5,   234, 29,  214, 73,  88,  67,  71,  77,  197, 179, 176, 251},
                                                     {144, 23,  243, 125, 99,  181, 70,  162, 173, 131, 216, 194, 159, 93,  13,  209, 12,  225},
                                                     {106, 171, 222, 100, 238, 13,  155, 112, 184, 10,  188, 125, 166, 188, 110, 241, 57,  64},
                                                     {1,248,74,14,131,186,148,0,242,175,209,226,97,118,119,22,176,90},
                                                     {123,191,179,212,33,62,199,211,72,197,182,241,213,60,192,175,77,131},
                                                     {57,168,57,12,64,57,102,79,133,248,165,118,199,13,11,217,138,146},
                                                     {208,255,140,169,28,116,19,116,228,217,150,90,55,175,67,97,163,107},
                                                     {151,152,58,109,189,228,237,13,150,107,177,154,130,129,197,105,49,199},
                                                     {63,222,78,98,14,30,56,8,24,61,67,42,28,249,13,201,52,136},
                                                     {32,112,165,143,167,126,29,152,5,26,150,26,225,176,132,40,149,149},
                                                     {96,73,149,103,199,37,215,20,139,187,28,30,128,51,122,11,14,56},
                                                     {179,134,186,45,185,184,33,106,146,198,90,196,198,46,213,168,10,161},
                                                     {207,21,80,205,130,163,97,163,162,49,230,4,73,228,142,77,4,241},
                                                     {37,209,148,19,170,183,63,143,227,7,80,92,24,178,39,164,217,141}};
    vector<vector<uint8_t>> ascon_emtpy_plaintext_cipher = {{98,143,117,52,151,231,18,252,35,34,224,0,181,52,44,52,93},
                                                     {127,42,203,55,78,126,158,91,232,202,205,70,80,13,74,134,189},
                                                     {167,211,226,59,48,56,99,214,249,41,204,146,248,234,171,191,160}};

    vector<vector<uint8_t>> aesgcm128_zero_plaintext_cipher = {{93,  48,  89,  54,  185, 246, 27, 40,  247, 242, 76, 18,  199, 185, 218, 43,  186, 244},
                                                               {20,  115, 60,  229, 246, 182, 34, 16,  59,  150, 37, 176, 86,  195, 197, 110, 150, 247},
                                                               {109, 233, 195, 245, 165, 177, 90, 185, 160, 30,  62, 243, 84,  215, 205, 208, 252, 136}};
}


 START_TEST(ecdh_empty){
    ecdh e(SECP256R1);
    const gfp_t private_key_zero = {{}};
    e.set_private_key(private_key_zero);

    const vector<uint8_t> data = e.get_data();
    const vector<uint8_t> shared_secret = e.get_shared_secret(data);
    vector<uint8_t> empty_vector;

    for(size_t i = 0; i < shared_secret.size(); i++) {
        empty_vector.push_back(0);
    }

    ck_assert_array_split_eq(shared_secret, empty_vector);
 }
 END_TEST

START_TEST(ecdh_keys_ff)
    {
        ecdh e_1(SECP256R1);
        ecdh e_2(SECP256R1);

        const gfp_t key_1 = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
        const gfp_t key_2 = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

        e_1.set_private_key(key_1);
        e_2.set_private_key(key_2);

        ck_assert_array_split_eq(e_1.get_shared_secret(e_2.get_data()), e_2.get_shared_secret(e_1.get_data()));
    }
END_TEST

START_TEST(ecdh_keys){
    ecdh e_1(SECP256R1);
    ecdh e_2(SECP256R1);

    const gfp_t key_1 = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const gfp_t key_2 = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1};

    e_1.set_private_key(key_1);
    e_2.set_private_key(key_2);

    ck_assert_array_split_eq(e_1.get_shared_secret(e_2.get_data()), e_2.get_shared_secret(e_1.get_data()));
    }
END_TEST

START_TEST(rec_lay_compare_handshake_traffic_keys_empty)
    {
        boost::asio::io_service io_service;

        for (connection_end c : {connection_end::CLIENT, connection_end::SERVER})
        {
            tcp::socket socket(io_service);
            tls_record_layer rec_lay(c, forward<tcp::socket>(socket));
            rec_lay.set_cipher_suite(TLS_ASCON_128_SHA256);
            const auto early_s = rec_lay.compute_early_secrets({}, {});
            rec_lay.compute_handshake_traffic_keys({}, {});

            ck_assert_array_split_eq(rec_lay.get_finished_key(connection_end::CLIENT), expected_client_finished_key);
            ck_assert_array_split_eq(rec_lay.get_finished_key(connection_end::SERVER), expected_server_finished_key);
        }
    }
END_TEST

START_TEST(rec_lay_compute_handshake_derived_key_empty)
    {
        boost::asio::io_service io_service;

        for (connection_end c : {connection_end::CLIENT, connection_end::SERVER})
        {
            tcp::socket socket(io_service);
            tls_record_layer record_layer(c, forward<tcp::socket>(socket));
            record_layer.set_cipher_suite(TLS_ASCON_128_SHA256);
            record_layer.compute_early_secrets({}, {});
            const auto derived_key = record_layer.compute_handshake_traffic_keys({}, {});

            ck_assert_array_split_eq(derived_key, expected_derived_key_empty);
        }
    }
END_TEST

START_TEST(rec_lay_early_secret_empty)
    {
        boost::asio::io_service io_service;

        for (connection_end c : {connection_end::CLIENT, connection_end::SERVER})
        {
            tcp::socket socket(io_service);
            tls_record_layer record_layer(c, forward<tcp::socket>(socket));
            record_layer.set_cipher_suite(TLS_ASCON_128_SHA256);

            const auto early_secret = record_layer.compute_early_secrets({}, {});

            ck_assert_array_split_eq(early_secret, expected_early_secret_empty);
        }
    }
END_TEST

START_TEST(ascon128_empty)
    {

        typename tls13_aesgcm::record expected_record;
        expected_record.header.type = TLS_APPLICATION_DATA;
        expected_record.header.version = TLSv1_2;
        expected_record.header.length = 17;

        hkdf kdf(salt, ikm);
        const auto key_data = kdf.expand(k_lab, ascon128::key_size);
        typename ascon128::key_storage ascon_key;
        copy(key_data.begin(), key_data.end(), ascon_key.begin());

        const auto nonce_data = kdf.expand(n_lab, ascon128::nonce_size);
        tls13_ascon tls(ascon_key, nonce_data);
        tls13_ascon tls_decrypt(ascon_key, nonce_data);

        for (size_t s = 0; s != 3; ++s){
            vector<uint8_t> plaintext;

            expected_record.ciphertext = ascon_emtpy_plaintext_cipher[s];
            const auto actual_record = tls.encrypt(TLS_APPLICATION_DATA, plaintext);



            if (actual_record != expected_record){
                ck_abort_msg("Records do not match.");
            }

            vector<uint8_t> decrypted_plaintext;
            content_type type;
            if (!tls_decrypt.decrypt(actual_record, decrypted_plaintext, type)){
                ck_abort_msg("Decryption failed.");
            }

            if (plaintext.size() != decrypted_plaintext.size() ||
                !equal(plaintext.begin(), plaintext.end(), decrypted_plaintext.begin())){
                ck_abort_msg("Plaintexts do not match.");
            }
        }
    }
END_TEST

START_TEST(ascon128_zero)
    {
        typename tls13_aesgcm::record expected_record;
        expected_record.header.type = TLS_APPLICATION_DATA;
        expected_record.header.version = TLSv1_2;
        expected_record.header.length = 18;

        hkdf kdf(salt, ikm);
        const auto key_data = kdf.expand(k_lab, ascon128::key_size);
        typename ascon128::key_storage ascon_key;
        copy(key_data.begin(), key_data.end(), ascon_key.begin());
        const auto nonce_data = kdf.expand(n_lab, ascon128::nonce_size);

        tls13_ascon tls(ascon_key, nonce_data);
        tls13_ascon tls_decrypt(ascon_key, nonce_data);

        for (size_t s = 0; s != 3; ++s){
            vector<uint8_t> plaintext;
            plaintext.push_back(0);

            expected_record.ciphertext = zero_plaintext_cipher[s];
            const auto actual_record = tls.encrypt(TLS_APPLICATION_DATA, plaintext);

            if (actual_record != expected_record){
                ck_abort_msg("Records do not match.");
            }

            vector<uint8_t> decrypted_plaintext;
            content_type type;
            if (!tls_decrypt.decrypt(actual_record, decrypted_plaintext, type)){
                ck_abort_msg("Decryption failed.");
            }

            if (plaintext.size() != decrypted_plaintext.size() ||
                !equal(plaintext.begin(), plaintext.end(), decrypted_plaintext.begin())){
                ck_abort_msg("Plaintexts do not match.");
            }
        }
    }
END_TEST

START_TEST(aes128gcm_empty)
    {
        typename tls13_aesgcm::record expected_record;
        expected_record.header.type = TLS_APPLICATION_DATA;
        expected_record.header.version = TLSv1_2;
        expected_record.header.length = 17;

        hkdf kdf(salt, ikm);
        const auto key_data = kdf.expand(k_lab, aes128gcm::key_size);
        typename aes128gcm::key_storage aesgcm_key;
        copy(key_data.begin(), key_data.end(), aesgcm_key.begin());

        const auto nonce_data = kdf.expand(n_lab, aes128gcm::nonce_size);
        tls13_aesgcm tls(aesgcm_key, nonce_data);
        tls13_aesgcm tls_decrypt(aesgcm_key, nonce_data);

        for (size_t s = 0; s != 15; ++s){
            expected_record.ciphertext = empty_plaintext_cipher[s];
            vector<uint8_t> plaintext;

            const auto actual_record = tls.encrypt(TLS_APPLICATION_DATA, plaintext);


            if (actual_record != expected_record){
                ck_abort_msg("Records do not match.");
            }

            vector<uint8_t> decrypted_plaintext;
            content_type type;
            if (!tls_decrypt.decrypt(actual_record, decrypted_plaintext, type)){
                ck_abort_msg("Decryption failed.");
            }

            if (plaintext.size() != decrypted_plaintext.size() ||
                !equal(plaintext.begin(), plaintext.end(), decrypted_plaintext.begin())){
                ck_abort_msg("Plaintexts do not match.");
            }
        }
    }
END_TEST

START_TEST(aes128gcm_zero)
    {
        typename tls13_aesgcm::record expected_record;
        expected_record.header.type = TLS_APPLICATION_DATA;
        expected_record.header.version = TLSv1_2;
        expected_record.header.length = 18;

        hkdf kdf(salt, ikm);
        const auto key_data = kdf.expand(k_lab, aes128gcm::key_size);
        typename aes128gcm::key_storage aesgcm_key;
        copy(key_data.begin(), key_data.end(), aesgcm_key.begin());
        const auto nonce_data = kdf.expand(n_lab, aes128gcm::nonce_size);

        tls13_aesgcm tls(aesgcm_key, nonce_data);
        tls13_aesgcm tls_decrypt(aesgcm_key, nonce_data);

        for (size_t s = 0; s != 3; ++s){
            vector<uint8_t> plaintext;
            plaintext.push_back(0);
            expected_record.ciphertext = aesgcm128_zero_plaintext_cipher[s];

            const auto actual_record = tls.encrypt(TLS_APPLICATION_DATA, plaintext);


            if (actual_record != expected_record){
                ck_abort_msg("Records do not match.");
            }

            vector<uint8_t> decrypted_plaintext;
            content_type type;
            if (!tls_decrypt.decrypt(actual_record, decrypted_plaintext, type)){
                ck_abort_msg("Decryption failed.");
            }

            if (plaintext.size() != decrypted_plaintext.size() ||
                !equal(plaintext.begin(), plaintext.end(), decrypted_plaintext.begin())){
                ck_abort_msg("Plaintexts do not match.");
            }
        }
    }
END_TEST

int main(int argc, char** argv)
{
    Suite* suite = suite_create("Student Task 2 Tests");

    TCase* tcase_ecdh = tcase_create("ecdh");
    tcase_add_test(tcase_ecdh, ecdh_empty);
    tcase_add_test(tcase_ecdh, ecdh_keys_ff);
    tcase_add_test(tcase_ecdh, ecdh_keys);
    suite_add_tcase(suite, tcase_ecdh);

    TCase* tcase_record = tcase_create("Record layer");
    tcase_add_test(tcase_record, rec_lay_compare_handshake_traffic_keys_empty);
    tcase_add_test(tcase_record, rec_lay_compute_handshake_derived_key_empty);
    tcase_add_test(tcase_record, rec_lay_early_secret_empty);
    suite_add_tcase(suite, tcase_record);

    TCase* tcase_encrypted = tcase_create("Record layer encrypted");
    tcase_add_test(tcase_encrypted, ascon128_empty);
    tcase_add_test(tcase_encrypted, ascon128_zero);
    tcase_add_test(tcase_encrypted, aes128gcm_empty);
    tcase_add_test(tcase_encrypted, aes128gcm_zero);
    suite_add_tcase(suite, tcase_encrypted);

    SRunner* suite_runner = srunner_create(suite);
    srunner_run(suite_runner, argc, argv);

    int number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    return !number_failed ? EXIT_SUCCESS : EXIT_FAILURE;
}
