#include "../random.h"
#include "../pir_client.h"
#include "../pir_server.h"
#include "../../utils/tests.h"
#include <check.h>

START_TEST(createQuery)
{
  uchar seed[16] = "pir_client_test";
  Random::Init(seed);

  // in practice desired privacy of 1 is not a good config, but faster to test :)
  pir_client client(2, 1, 10);
  client.setUpClient();
  const auto queries = client.clientQuery(3);

  const auto ref_1 = BigInteger2048::FromFile(SOURCE_DIR "data/pir_client_query_1.txt");
  const auto ref_2 = BigInteger2048::FromFile(SOURCE_DIR "data/pir_client_query_2.txt");

  ck_assert_int_eq(queries[0].size(), ref_1.size());
  for(size_t i = 0; i < ref_1.size(); i++)
  {
    ck_assert_bi_eq(queries[0][i], ref_1[i]);
  }
  ck_assert_int_eq(queries[1].size(), ref_2.size());
  for(size_t i = 0; i < ref_2.size(); i++)
  {
    ck_assert_bi_eq(queries[1][i], ref_2[i]);
  }
}
END_TEST

START_TEST(clientFinish)
{
  uchar seed[16] = "pir_client_test";
  Random::Init(seed);

  // in practice desired privacy of 1 is not a good config, but faster to test :)
  pir_client client(2, 1, 10);
  client.setUpClient();
  client.clientQuery(3);

  const auto server_response_1 = BigInteger2048::FromFile(SOURCE_DIR "data/pir_server_response_1.txt");
  const auto server_response_2 = BigInteger2048::FromFile(SOURCE_DIR "data/pir_server_response_2.txt");
  std::vector<std::vector<BigInteger2048>> responses = {server_response_1, server_response_2};
  const auto ref = BigInteger1024::FromFile(SOURCE_DIR "data/pir_client_result.txt");

  const auto result = client.clientFinish(responses);

  ck_assert_int_eq(result.size(), ref.size());
  for(size_t i = 0; i < ref.size(); i++)
  {
    ck_assert_bi_eq(result[i], ref[i]);
  }
}
END_TEST

START_TEST(serverResponse)
{
  pir_server server(10, 19968/127);

  const auto query = BigInteger2048::FromFile(SOURCE_DIR "data/pir_client_query_1.txt");
  const auto ref = BigInteger2048::FromFile(SOURCE_DIR "data/pir_server_response_1.txt");

  const auto response = server.serverResponse(query);

  ck_assert_int_eq(response.size(), ref.size());
  for(size_t i = 0; i < response.size(); i++)
  {
    ck_assert_bi_eq(response[i], ref[i]);
  }
}
END_TEST

START_TEST(executeProtocol)
{
  utils::executeProtocol(2, 1 ,10, 0, "result.jpg");
}
END_TEST

int main(int argc, char** argv)
{
  Suite* suite = suite_create("pir");

  TCase* tcase = tcase_create("pir_client");
  tcase_add_test(tcase, createQuery);
  tcase_add_test(tcase, clientFinish);
  tcase_set_timeout(tcase, 120);
  suite_add_tcase(suite, tcase);

  tcase = tcase_create("pir_server");
  tcase_set_timeout(tcase, 120);
  tcase_add_test(tcase, serverResponse);
  suite_add_tcase(suite, tcase);

  tcase = tcase_create("execute protocol");
  tcase_set_timeout(tcase, 240);
  tcase_add_test(tcase, executeProtocol);
  suite_add_tcase(suite, tcase);

  SRunner* suite_runner = srunner_create(suite);
  srunner_run(suite_runner, argc, argv);
  int number_failed = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);

  return !number_failed ? EXIT_SUCCESS : EXIT_FAILURE;
}
