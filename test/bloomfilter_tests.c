#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/bloomfilter/bloomfilter.h"

void testBloomfilterCreate(void) {
    char* file_path = "test/data/test.txt";
    CategoryList* list = parse_categories(file_path);

    Bloomfilter* bloomfilter = create_bloomfilter(list, -1, 8); // chance of false positive = 1/256
    uint32_t n = 39; // amount of bits / 8 = 39 * 8 / 8
    uint8_t k = 5; // amount of hash functions = 8 * ln(2) rounded down
    TEST_CHECK(bloomfilter->size == n);
    TEST_CHECK(bloomfilter->hash_amount == k);

    char* element_in = "element3";
    uint8_t count = 0;
    for (int8_t h = 0; h < k; h++) {
        uint32_t hash = murmurhash(element_in, 8, bloomfilter->hash_seeds[h]) % (n * 8);
        if (bloomfilter->bf[hash / 8] & (1ULL << (hash % 8))) count++;
    }
    TEST_CHECK(count == k);

    char* element_out = "element7";
    count = 0;
    for (int8_t h = 0; h < k; h++) {
        uint32_t hash = murmurhash(element_out, 8, bloomfilter->hash_seeds[h]) % (n * 8);
        if (bloomfilter->bf[hash / 8] & (1ULL << (hash % 8))) count++;
    }
    TEST_CHECK(count < k);

    free_categories(list);
    free_bloomfilter(bloomfilter);
}

void testBloomfilterFailcheck(void) {
    char* file_path = "test/data/test.txt";
    CategoryList* list = parse_categories(file_path);

    // should return an empty bloomfilter, not throw an error
    Bloomfilter* bloomfilter = create_bloomfilter(list, -1, 0);
    TEST_CHECK(bloomfilter->size == 0);
    TEST_CHECK(bloomfilter->bf != NULL);

    free_categories(list);
    free_bloomfilter(bloomfilter);
}