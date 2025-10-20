#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/bloomfilter/bloomfilter.h"

void testBloomfilterCreate(void) {
    char* file_path = "test/data/test.txt";
    CategoryList* list = parse_categories(file_path);

    Bloomfilter* bloomfilter = create_bloomfilter(list, -1, 3);
    TEST_CHECK(bloomfilter->size == 15); // amount of bits = 15 * 8

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