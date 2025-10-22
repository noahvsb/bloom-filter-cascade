#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/hash/murmurhash.h"
#include <string.h>

#define SEED 123
#define N 64

void test_murmurhash(void) {
    char* test = "test1";
    uint8_t hash1 = murmurhash(test, strlen(test), SEED) % N;
    TEST_CHECK(hash1 < N);

    test = "test2";
    uint8_t hash2 = murmurhash(test, strlen(test), SEED) % N;
    TEST_CHECK(hash2 < N);

    TEST_CHECK(hash1 != hash2);
}