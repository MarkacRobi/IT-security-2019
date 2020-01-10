//
// Created by Edi Muskardin on 03/12/2019.
//

#include "pir_server.h"
#ifdef PROTOCOLS
#include "bigint_ref/BigInteger2048.h"
#else
#include "bigint/BigInteger2048.h"
#endif


std::vector<BigInteger2048> pir_server::serverResponse(const std::vector<BigInteger2048>& encPi) {
}
