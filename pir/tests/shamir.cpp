#include "../../utils/tests.h"
#include <check.h>
#include <iostream>
#include <fstream>
#include "../shamir.h"
#include "../bigint/BigInteger1024.h"
#include "../random.h"

namespace {
  const BigInteger1024 secret_1_tc_1(42);

  const std::vector<BigInteger1024> indices = {
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

START_TEST(random_small){
  uint8_t degree = 3;

  BigInteger1024 secret(0);
  shamir test(degree);
  test.generateIndices(4);

  std::vector<BigInteger1024> polynomial = test.generateShamirPolynomial(secret);
  std::vector<BigInteger1024> shares = test.generateShamirShares();
  BigInteger1024 rec_secret = test.reconstructSecret(shares);

  ck_assert_bi_eq(secret, rec_secret);
}END_TEST

START_TEST(polynomial_generation){
  uint8_t degree = 5;
  BigInteger1024 secret(13);
  shamir test1(degree);
  test1.generateIndices(degree);
  std::vector<BigInteger1024> polynomial1 = test1.generateShamirPolynomial(secret);
  std::vector<BigInteger1024> polynomial2 = test1.generateShamirPolynomial(secret);

  ck_assert_int_ne(polynomial1.size(), 0);
  ck_assert_int_ne(polynomial2.size(), 0);
  ck_assert_int_eq(polynomial1.size(), polynomial2.size());

  for(size_t i = 0; i < polynomial1.size(); i++){
    if(i == 0){
      ck_assert_bi_eq(polynomial1.at(i), polynomial2.at(i));
    }
    else
      ck_assert_bi_ne(polynomial1.at(i), polynomial2.at(i));
  }
}END_TEST

START_TEST(generate_polynomial)
{
  uchar seed[16] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  Random::Init(seed);
  uint8_t degree = 5;
  BigInteger1024 secret(13);
  shamir test(degree);
  std::vector<BigInteger1024> polynomial = test.generateShamirPolynomial(secret);

  std::vector<BigInteger1024> reference = BigInteger1024::FromFile(SOURCE_DIR "data/shamir_generate_polynomial.txt");

  ck_assert_int_eq(polynomial.size(), reference.size());
  for(size_t i = 0; i < polynomial.size(); ++i)
  {
    ck_assert_bi_eq(polynomial.at(i), reference.at(i));
  }

}
END_TEST

START_TEST(generate_shares)
{
  uint8_t degree = 5;
  BigInteger1024 secret(13);
  uchar seed[16] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  Random::Init(seed);
  shamir test(degree);
  test.generateShamirPolynomial(secret);
  test.setIndices(indices);
  std::vector<BigInteger1024> shares = test.generateShamirShares();
  
  std::vector<BigInteger1024> expected = {
    BigInteger1024("27402698d4cd895574ba982a5dda11683046ec1582fa873de8b39df88c99fabeed9a2d5ca58a1bd00ebe4920563ceb90526431189d3e054164bcf1dcc1c446de895f4c2987b0704174d314f80f4bbc1793e2aa05c42d07ade2639c1a62e66c4349d68a9586b5e7a6e64354532512e236770a0c74cf44dbfe837855575ca16210"),
    BigInteger1024("a7e5315c84bbd6dee564017a3bcfca669276ef939e8aa91c9c69261c6a28eab7713343affd8c5e376bbcc069c10ab376e757a5e0ea13150ab36f689b96faf758e178d44bd350f7be012838cc7ba8482bfa7456e3c0de553641af6db76274baf477eace7a5c2efb117029a10f71836bd6f9af4354ca334581d4573ec2cb252594"),
    BigInteger1024("54061c71b1660cce264130244275db35bfad753006d37c184cf2f5c599ac451c5593f98ef40b83739d7b217d1ea8e8135a4d967a502dfbea04a60523fd858ee114cf90f00d98eb4a62a52681733dced47ef71c6e4ad410d31d9fd2968082caa2599429f352413b68976d24a38819213402c5abe9c059307ef114dc9c50e0332d"),
    BigInteger1024("728dcb93351ed18021a0009077d995f093321d7f15fe0ad9f86e620dc75067f2a41df5b570d269af699255928fccd735d8edf8b6ea97e12c8f91a0a2aa8aca77ae02e4cb7ada9d3ff81e55bcb74f16083968105cfc7bd2302b7d119e93a2458e7bcdb208161c3eef7d2d962abda544cd67c9ce9d611f0d8988b076c3b2af8c0a"),
    BigInteger1024("7d814dac085b421b74f69cc7dc2c1e8332e9ad9c6ac0f02baf5dc7e9710f22e9487d39a6dd8894bf1c80055952043445b53df462c6bf728843c24b36a933ed1059d62e00be4652da6be66602bf37bce4adb4eb00d7c743d3af6c8a3a37bceefb2a50221439e4e1097ce10ce3e5ba231c74a0523cb334f9946934bdeba9ea2b56"),
    BigInteger1024("979642ff467150518a9e73b160252ecdf4ad5e4a88c3dafd152f221a7b24d6458e1e6338d859a405f0c713a1ca484c0efce7b16af565613625dbfcc0f9a055bffd20a7650ed127a64b775ec4bb6ab0c4985012d3e51ee9b863e1c120d984748a91bbad5cbcead0ce422ae6790d818da40705aac1774b340383fd0d503de4249b"),
    BigInteger1024("7445a77ceb43a80209f0842e12152e4abe4ff6a3e7f2b9c0c5fbf78fc550c3607bc099e9211d809df882970f929051f3bcc7c359bc0ded4a0508f705d7ae34f52084835390a5f9dbc719b884ee8156926366e6d352e4f3b28b631f184643a74a887732cbb3b63d0f3d1c7b2db6dd1e613dd8e57d2ae81867926cb39f0c4aed1a"),
    BigInteger1024("0bddff29dbbc44a9eaa3a730f8114379c791516ce49632bf5c9c60f5716cc376d0aa158f03b7f495542359ff0342ba925eef1f815ef8cc1f55184c6ada08c8aaf8529743ff15cb70bd1f703e568c3bc6e68892c25aff58ecf5192a1d8ecf0605767d3dcec0d982601d11e23a7b40a053dbe44b22cf970488eb6a1a789040b72a"),
  };
  
  for(size_t i = 0; i < expected.size(); ++i)
  {
    ck_assert_bi_eq(shares.at(i), expected.at(i));
  }
}
END_TEST

START_TEST(reconstruct_fixed_small){
  uint8_t degree = 4;
  shamir test(degree);
  test.setIndices(std::vector<BigInteger1024>(indices.begin(), indices.begin()+5));

  std::vector<BigInteger1024> fixed_shares = BigInteger1024::FromFile(SOURCE_DIR "data/shamir_fixed_1.txt");

  BigInteger1024 res = test.reconstructSecret(fixed_shares);

  ck_assert_bi_eq(res, secret_1_tc_1);
}END_TEST

START_TEST(share_0_and_1){
  uint8_t degree = 5;
  shamir test(degree);
  BigInteger1024 zero(0);
  BigInteger1024 one(1);

  test.generateShamirPolynomial(zero);
  test.setIndices(std::vector<BigInteger1024>(indices.begin(), indices.begin()+degree+1));
  std::vector<BigInteger1024> shares_1 = test.generateShamirShares();

  shamir test1(degree);
  test1.generateShamirPolynomial(one);
  test1.setIndices(std::vector<BigInteger1024>(indices.begin(), indices.begin()+degree+1));
  std::vector<BigInteger1024> shares_2 = test1.generateShamirShares();


  BigInteger1024 rec_zero = test.reconstructSecret(shares_1);
  BigInteger1024 rec_one = test1.reconstructSecret(shares_2);

  ck_assert_bi_eq(zero, rec_zero);
  ck_assert_bi_eq(one, rec_one);
}END_TEST



int main(int argc, char* argv[]){
  Suite* suite = suite_create("Shamir Secretsharing");

  TCase* tcase = tcase_create("Functionality");
  tcase_add_test(tcase, polynomial_generation);
  tcase_add_test(tcase, generate_polynomial);
  tcase_add_test(tcase, generate_shares);
  tcase_add_test(tcase, random_small);
  tcase_add_test(tcase, reconstruct_fixed_small);
  tcase_add_test(tcase, share_0_and_1);

  suite_add_tcase(suite, tcase);

  SRunner* suite_runner = srunner_create(suite);
  srunner_run(suite_runner, argc, argv);
  int number_failed = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);

  return !number_failed ? EXIT_SUCCESS : EXIT_FAILURE;
}
