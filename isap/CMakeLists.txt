set(ISAP_SOURCES
    Ascon-reference.c
    decrypt.c
    encrypt.c
    isap.c
    permutation.c)

add_library(isap SHARED ${ISAP_SOURCES})
