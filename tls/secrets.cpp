#include "secrets.h"
#include "../utils/utils.h"

using util::operator""_x;
using util::operator""_b;

namespace secrets
{
  const psk_map client_psks{
      {"identity", "abab"_x},
      {"test-identity0", "cdcd"_x},
      {"test-identity1", "6162636465666768696a6b6c6d6e6f707172737475767778797a"_x},
      {"test-identity2",
       "49545320536563757269747920746573742d6964656e74697479322070617373776f7264"_x}};

  const psk_map server_psks{
      {"identity", "abab"_x},
      {"test-identity1", "616263646566676869600000006e6f707172737475767778797a"_x},
      {"test-identity2",
       "49545320536563757269747920746573742d6964656e74697479322070617373776f7264"_x},
      {"test-identity3", "0102"_x}};

  const std::array<std::vector<uint8_t>, 10> application_data{
      "O Freunde, nicht diese Toene!\n"
      "Sondern lasst uns angenehmere\n"
      "anstimmen und freudenvollere.\n"_b,
      "Freude! Freude!\n"_b,
      "Freude, schoener Goetterfunken,\n"
      "Tochter aus Elysium,\n"
      "Wir betreten feuertrunken,\n"
      "Himmlische, dein Heiligthum!\n"
      "Deine Zauber binden wieder\n"
      "Was die Mode streng geteilt;\n"
      "Alle Menschen werden Brueder,\n"
      "Wo dein sanfter Fluegel weilt.\n"_b,
      "Wem der grosse Wurf gelungen,\n"
      "Eines Freundes Freund zu sein;\n"
      "Wer ein holdes Weib errungen,\n"
      "Mische seinen Jubel ein!\n"_b,
      "Ja, wer auch nur eine Seele\n"
      "Sein nennt auf dem Erdenrund!\n"
      "Und wers nie gekonnt, der stehle\n"
      "Weinend sich aus diesem Bund!\n"_b,
      "Freude trinken alle Wesen\n"
      "An den Bruesten der Natur;\n"
      "Alle Guten, alle Boesen\n"
      "Folgen ihrer Rosenspur.\n"_b,
      "Kuesse gab sie uns und Reben,\n"
      "Einen Freund, geprueft im Tod;\n"
      "Wollust ward dem Wurm gegeben,\n"
      "Und der Cherub steht vor Gott.\n"_b,
      "Froh, wie seine Sonnen fliegen\n"
      "Durch des Himmels praechtgen Plan,\n"
      "Laufet, Brueder, eure Bahn,\n"
      "Freudig, wie ein Held zum Siegen.\n"_b,
      "Seid umschlungen, Millionen!\n"
      "Diesen Kuss der ganzen Welt!\n"
      "Brueder, ueberm Sternenzelt\n"
      "Muss ein lieber Vater wohnen.\n"
      "Ihr stuerzt nieder, Millionen?\n"
      "Ahnest du den Schoepfer, Welt?\n"
      "Such ihn ueberm Sternenzelt!\n"
      "Ueber Sternen muss er wohnen.\n"_b,
      "Seid umschlungen, Millionen!\n"
      "Diesen Kuss der ganzen Welt!\n"
      "Brueder, ueberm Sternenzelt\n"
      "Muss ein lieber Vater wohnen.\n"
      "Seid umschlungen, Millionen!\n"
      "Diesen Kuss der ganzen Welt!\n"
      "Freude, schoener Goetterfunken\n"
      "Tochter aus Elysium,\n"
      "Freude, schoener Goetterfunken, Goetterfunken.\n"_b};

} // namespace secrets
