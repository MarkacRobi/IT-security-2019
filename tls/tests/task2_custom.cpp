#include "../../utils/tests.h"
#include "../../utils/utils.h"
#include <boost/asio.hpp>
#include "../tls-record-layer.h"
#include <string>
#include <check.h>
#include "../ecdh.h"
#include "../secrets.h"

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

int main(int argc, char** argv)
{
    Suite* suite = suite_create("Student Task 2 Tests");

    TCase* tcase_ecdh = tcase_create("ecdh");
    tcase_add_test(tcase_ecdh, ecdh_empty);
    tcase_add_test(tcase_ecdh, ecdh_keys);
    suite_add_tcase(suite, tcase_ecdh);

    TCase* tcase_record = tcase_create("Record layer");
    tcase_add_test(tcase_record, rec_lay_compare_handshake_traffic_keys_empty);
    tcase_add_test(tcase_record, rec_lay_compute_handshake_derived_key_empty);
    tcase_add_test(tcase_record, rec_lay_early_secret_empty);
    suite_add_tcase(suite, tcase_record);

    SRunner* suite_runner = srunner_create(suite);
    srunner_run(suite_runner, argc, argv);

    int number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    return !number_failed ? EXIT_SUCCESS : EXIT_FAILURE;
}
