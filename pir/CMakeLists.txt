if(PROTOCOLS)
  set(PRIMITIVES_SOURCES
    shamir.cpp
    paillier.cpp
    pir_client.cpp
    pir_server.cpp
    utils.cpp
  )
else()
  set(PRIMITIVES_SOURCES
    bigint/BigInteger.cpp
    bigint/BigInteger1024.cpp
    bigint/BigInteger2048.cpp
    shamir.cpp
    paillier.cpp
    pir_client.cpp
    pir_server.cpp
    utils.cpp
    random.cpp
    ../tls/aes/aes128.cpp
    )
endif()

add_library(pir_primitives SHARED ${PRIMITIVES_SOURCES})

add_definitions("-DDATABASE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/database/\"")

if(CXX_SUPPORTS_MAES)
  target_compile_options(pir_primitives PRIVATE "-maes")
  target_compile_definitions(pir_primitives PRIVATE -DHAVE_AESNI)
endif()

add_subdirectory(tests)
