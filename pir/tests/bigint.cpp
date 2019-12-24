#include"../../utils/tests.h"
#include"../bigint/BigInteger2048.h"
#include"../bigint/BigInteger1024.h"
#include"../bigint/BigInteger.h"

#include<check.h>
#include<cstdlib>


START_TEST(addition2048)
{
  auto as = BigInteger2048::FromFile(SOURCE_DIR "data/as.txt");
  auto bs = BigInteger2048::FromFile(SOURCE_DIR "data/bs.txt");
  auto cs = BigInteger2048::FromFile(SOURCE_DIR "data/sums_mod.txt");
  
  for(unsigned int i = 0; i < as.size(); i++)
  {
    auto res = as[i] + (bs[i] + BigInteger2048(0) + BigInteger2048(0));
    ck_assert_bi_eq(res, cs[i]);
  }
}
END_TEST


START_TEST(subtraction2048)
{
  auto as = BigInteger2048::FromFile(SOURCE_DIR "data/as.txt");
  auto bs = BigInteger2048::FromFile(SOURCE_DIR "data/bs.txt");
  auto cs = BigInteger2048::FromFile(SOURCE_DIR "data/diffs_mod.txt");
  
  for(unsigned int i = 0; i < as.size(); i++)
  {
    auto res = as[i] - (bs[i] + BigInteger2048(0) + BigInteger2048(0));
    ck_assert_bi_eq(res, cs[i]);
  }
}
END_TEST


START_TEST(multiplication2048)
{
  auto as = BigInteger2048::FromFile(SOURCE_DIR "data/as.txt");
  auto bs = BigInteger2048::FromFile(SOURCE_DIR "data/bs.txt");
  auto cs = BigInteger2048::FromFile(SOURCE_DIR "data/cs.txt");
  
  for(unsigned int i = 0; i < as.size(); i++)
  {
    auto res = as[i] * bs[i];
    ck_assert_bi_eq(res, cs[i]);
  }
}
END_TEST

START_TEST(powermod2048)
{
  auto as = BigInteger2048::FromFile(SOURCE_DIR "data/as.txt");
  auto bs = BigInteger2048::FromFile(SOURCE_DIR "data/bs.txt");
  auto es = BigInteger2048::FromFile(SOURCE_DIR "data/es.txt");
  
  for(unsigned int i = 0; i < as.size(); i++)
  {
    auto res = BigInteger2048::PowerMod(as[i], bs[i]);
    ck_assert_bi_eq(res, es[i]);
  }
}
END_TEST

START_TEST(division2048)
{
  auto xs = BigInteger2048::FromFile(SOURCE_DIR "data/xs.txt");
  auto ys = BigInteger1024::FromFile(SOURCE_DIR "data/ys.txt");
  auto refs = BigInteger1024::FromFile(SOURCE_DIR "data/quot.txt");
  
  for(unsigned int i = 0; i < xs.size(); i++)
  {
    auto res = xs[i] / ys[i];
    ck_assert_bi_eq(res, refs[i]);
  }
}
END_TEST


START_TEST(addition1024)
{
  auto as = BigInteger1024::FromFile(SOURCE_DIR "data/as1024.txt");
  auto bs = BigInteger1024::FromFile(SOURCE_DIR "data/bs1024.txt");
  auto cs = BigInteger1024::FromFile(SOURCE_DIR "data/sums_mod1024.txt");
  
  for(unsigned int i = 0; i < as.size(); i++)
  {
    auto res = as[i] + (bs[i] + BigInteger1024(0) + BigInteger1024(0));
    ck_assert_bi_eq(res, cs[i]);
  }
}
END_TEST


START_TEST(subtraction1024)
{
  auto as = BigInteger1024::FromFile(SOURCE_DIR "data/as1024.txt");
  auto bs = BigInteger1024::FromFile(SOURCE_DIR "data/bs1024.txt");
  auto cs = BigInteger1024::FromFile(SOURCE_DIR "data/diffs_mod1024.txt");
  
  for(unsigned int i = 0; i < as.size(); i++)
  {
    auto res = as[i] - (bs[i] + BigInteger1024(0) + BigInteger1024(0));
    ck_assert_bi_eq(res, cs[i]);
  }
}
END_TEST

START_TEST(multiplication1024)
{
  auto as = BigInteger1024::FromFile(SOURCE_DIR "data/as1024.txt");
  auto bs = BigInteger1024::FromFile(SOURCE_DIR "data/bs1024.txt");
  auto cs = BigInteger1024::FromFile(SOURCE_DIR "data/cs1024.txt");
  
  for(unsigned int i = 0; i < as.size(); i++)
  {
    auto res = as[i] * bs[i];
    ck_assert_bi_eq(res, cs[i]);
  }
}
END_TEST

START_TEST(powermod1024)
{
  auto as = BigInteger1024::FromFile(SOURCE_DIR "data/as1024.txt");
  auto bs = BigInteger1024::FromFile(SOURCE_DIR "data/bs1024.txt");
  auto es = BigInteger1024::FromFile(SOURCE_DIR "data/es1024.txt");
  
  for(unsigned int i = 0; i < as.size(); i++)
  {
    auto res = BigInteger1024::PowerMod(as[i], bs[i]);
    ck_assert_bi_eq(res, es[i]);
  }
}
END_TEST

START_TEST(inverse1024)
{
  auto as = BigInteger1024::FromFile(SOURCE_DIR "data/as1024.txt");
  auto bs = BigInteger1024::FromFile(SOURCE_DIR "data/bs1024.txt");
  auto invs = BigInteger1024::FromFile(SOURCE_DIR "data/invs1024.txt");
  
  for(unsigned int i = 0; i < as.size(); i++)
  {
    auto res = BigInteger1024::Inverse(as[i]);
    ck_assert_bi_eq(res, invs[i]);
  }
}
END_TEST


START_TEST(add)
{
  auto as = BigInteger1024::FromFile(SOURCE_DIR "data/as1024.txt");
  auto bs = BigInteger1024::FromFile(SOURCE_DIR "data/bs1024.txt");
  
  auto sums = BigInteger2048::FromFile(SOURCE_DIR "data/sums.txt");
  
  for(unsigned int i = 0; i < as.size(); i++)
  {
    const auto& a = as.at(i);
    const auto& b = bs.at(i);
    BigInteger2048 c;
    BigInteger::AddIntegers((word*)c.GetData(), (word*)a.GetData(), (word*)b.GetData(), (word*)(c.GetData() + NUM_BYTES_1024), NUM_WORDS_1024);
        
    ck_assert_bi_eq(c, sums.at(i));
  }
}
END_TEST

START_TEST(subtract)
{
  auto as = BigInteger1024::FromFile(SOURCE_DIR "data/as1024.txt");
  auto bs = BigInteger1024::FromFile(SOURCE_DIR "data/bs1024.txt");
  
  auto diffs = BigInteger2048::FromFile(SOURCE_DIR "data/diffs.txt");
  
  for(unsigned int i = 0; i < as.size(); i++)
  {
    const auto& a = as.at(i);
    const auto& b = bs.at(i);
    BigInteger2048 c;
    BigInteger::SubtractIntegers((word*)c.GetData(), (word*)a.GetData(), (word*)b.GetData(), (word*)(c.GetData() + NUM_BYTES_1024), NUM_WORDS_1024);
        
    ck_assert_bi_eq(c, diffs.at(i));
  }
}
END_TEST

START_TEST(multiply)
{
  auto as = BigInteger1024::FromFile(SOURCE_DIR "data/as1024.txt");
  auto bs = BigInteger1024::FromFile(SOURCE_DIR "data/bs1024.txt");
  
  auto prods = BigInteger2048::FromFile(SOURCE_DIR "data/prods.txt");
  
  for(unsigned int i = 0; i < as.size(); i++)
  {
    const auto& a = as.at(i);
    const auto& b = bs.at(i);
    BigInteger2048 c;
    BigInteger::MultiplyIntegers((word*)c.GetData(), (word*)a.GetData(), (word*)b.GetData(), NUM_WORDS_1024);
        
    ck_assert_bi_eq(c, prods.at(i));
  }
}
END_TEST

START_TEST(comparison)
{
  auto a1 = BigInteger1024::FromFile(SOURCE_DIR "data/as1024.txt");
  auto a2 = BigInteger2048::FromFile(SOURCE_DIR "data/as.txt");
  auto comp = BigInteger2048::FromFile(SOURCE_DIR "data/comp.txt");
  
  ck_assert_int_eq(BigInteger::GreaterThan((word*)a1[0].GetData(), (word*)a1[0].GetData(), NUM_WORDS_1024, NUM_WORDS_1024), false);
  ck_assert_int_eq(BigInteger::GreaterThan((word*)a1[0].GetData(), (word*)a1[3].GetData(), NUM_WORDS_1024, NUM_WORDS_1024), true);
  ck_assert_int_eq(BigInteger::GreaterThan((word*)a1[4].GetData(), (word*)a1[5].GetData(), NUM_WORDS_1024, NUM_WORDS_1024), false);
  ck_assert_int_eq(BigInteger::GreaterThan((word*)a1[6].GetData(), (word*)a1[7].GetData(), NUM_WORDS_1024, NUM_WORDS_1024), true);
  
  ck_assert_int_eq(BigInteger::GreaterThan((word*)a1[0].GetData(), (word*)a2[3].GetData(), NUM_WORDS_1024, NUM_WORDS_2048), true);
  ck_assert_int_eq(BigInteger::GreaterThan((word*)a2[6].GetData(), (word*)a1[6].GetData(), NUM_WORDS_2048, NUM_WORDS_1024), true);

  ck_assert_int_eq(BigInteger::GreaterThan((word*)a2[0].GetData(), (word*)a2[0].GetData(), NUM_WORDS_2048, NUM_WORDS_2048), false);
  ck_assert_int_eq(BigInteger::GreaterThan((word*)a2[0].GetData(), (word*)a2[3].GetData(), NUM_WORDS_2048, NUM_WORDS_2048), true);
  ck_assert_int_eq(BigInteger::GreaterThan((word*)a2[4].GetData(), (word*)a2[11].GetData(), NUM_WORDS_2048, NUM_WORDS_2048), false);
  ck_assert_int_eq(BigInteger::GreaterThan((word*)a2[6].GetData(), (word*)a2[7].GetData(), NUM_WORDS_2048, NUM_WORDS_2048), true);

  ck_assert_int_eq(BigInteger::GreaterThan((word*)comp[0].GetData(), (word*)comp[1].GetData(), NUM_WORDS_2048, NUM_WORDS_2048), false);
  ck_assert_int_eq(BigInteger::GreaterThan((word*)comp[2].GetData(), (word*)comp[3].GetData(), NUM_WORDS_2048, NUM_WORDS_2048), true);

  ck_assert_int_eq(BigInteger::SmallerThan((word*)a1[0].GetData(), (word*)a1[0].GetData(), NUM_WORDS_1024, NUM_WORDS_1024), false);
  ck_assert_int_eq(BigInteger::SmallerThan((word*)a1[0].GetData(), (word*)a1[3].GetData(), NUM_WORDS_1024, NUM_WORDS_1024), false);
  ck_assert_int_eq(BigInteger::SmallerThan((word*)a1[4].GetData(), (word*)a1[5].GetData(), NUM_WORDS_1024, NUM_WORDS_1024), true);
  ck_assert_int_eq(BigInteger::SmallerThan((word*)a1[6].GetData(), (word*)a1[7].GetData(), NUM_WORDS_1024, NUM_WORDS_1024), false);
  
  ck_assert_int_eq(BigInteger::SmallerThan((word*)a1[0].GetData(), (word*)a2[3].GetData(), NUM_WORDS_1024, NUM_WORDS_2048), false);
  ck_assert_int_eq(BigInteger::SmallerThan((word*)a2[6].GetData(), (word*)a1[6].GetData(), NUM_WORDS_2048, NUM_WORDS_1024), false);

  ck_assert_int_eq(BigInteger::SmallerThan((word*)a2[0].GetData(), (word*)a2[0].GetData(), NUM_WORDS_2048, NUM_WORDS_2048), false);
  ck_assert_int_eq(BigInteger::SmallerThan((word*)a2[0].GetData(), (word*)a2[3].GetData(), NUM_WORDS_2048, NUM_WORDS_2048), false);
  ck_assert_int_eq(BigInteger::SmallerThan((word*)a2[4].GetData(), (word*)a2[11].GetData(), NUM_WORDS_2048, NUM_WORDS_2048), true);
  ck_assert_int_eq(BigInteger::SmallerThan((word*)a2[6].GetData(), (word*)a2[7].GetData(), NUM_WORDS_2048, NUM_WORDS_2048), false);

  ck_assert_int_eq(BigInteger::SmallerThan((word*)comp[0].GetData(), (word*)comp[1].GetData(), NUM_WORDS_2048, NUM_WORDS_2048), true);
  ck_assert_int_eq(BigInteger::SmallerThan((word*)comp[2].GetData(), (word*)comp[3].GetData(), NUM_WORDS_2048, NUM_WORDS_2048), false);
}
END_TEST

int main(int argc, char** argv)
{
  Suite* suite = suite_create("BigInteger");

  TCase* tcase = tcase_create("BigInteger");
  tcase_add_test(tcase, add);
  tcase_add_test(tcase, subtract);
  tcase_add_test(tcase, multiply);
  tcase_add_test(tcase, comparison);
  suite_add_tcase(suite, tcase);

  tcase = tcase_create("BigInteger2048");
  tcase_add_test(tcase, addition2048);
  tcase_add_test(tcase, subtraction2048);
  tcase_add_test(tcase, multiplication2048);
  tcase_add_test(tcase, powermod2048);
  tcase_add_test(tcase, division2048);
  suite_add_tcase(suite, tcase);
  
  tcase = tcase_create("BigInteger1024");
  tcase_add_test(tcase, addition1024);
  tcase_add_test(tcase, subtraction1024);
  tcase_add_test(tcase, multiplication1024);
  tcase_add_test(tcase, powermod1024);
  tcase_add_test(tcase, inverse1024);
  suite_add_tcase(suite, tcase);

  SRunner* suite_runner = srunner_create(suite);
  srunner_run(suite_runner, argc, argv);
  int number_failed = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);

  return !number_failed ? EXIT_SUCCESS : EXIT_FAILURE;
}
