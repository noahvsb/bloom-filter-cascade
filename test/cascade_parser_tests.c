#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/file/cascade_parser.h"

void test_parse_cascade(void) {
    char* file_path = "test/data/test.file"; // bloomfilter cascade of test/data/test.txt with p = 1

    Cascade* cascade = parse_cascade(file_path);
    TEST_CHECK(cascade->categories_size == 5);
    
    for (uint32_t i = 0; i < cascade->categories_size; i++) {
        char expected[32];
        sprintf(expected, "category%u", i);

        TEST_CHECK(strcmp(cascade->categories_names[i], expected) == 0);
    }

    TEST_CHECK(cascade->cascade_steps == 2);

    for (uint32_t i = 0; i < cascade->cascade_steps * cascade->categories_size; i++) {
        Bloomfilter* bloomfilter = cascade->bloomfilters[i];
        TEST_CHECK(bloomfilter == NULL || (bloomfilter->hash_amount == 1 && bloomfilter->size > 0));
        // actual content of bloomfilters will be checked in classify_test.c
    }
  
    // no last category name, all were empty after the second step (makes sense because there weren't a lot of elements)
    TEST_CHECK(cascade->last_category_name == NULL);

    free_cascade(cascade);
}

void test_parse_cascade_failcheck(void) {
    printf("\n");

    char* file_path = "test/data/unknown.file";
    Cascade* cascade = parse_cascade(file_path);
    TEST_CHECK(cascade == NULL);

    free_cascade(cascade);

    file_path = "test/data/temp.file";

    // create an empty file
    fclose(fopen(file_path, "wb"));

    cascade = parse_cascade(file_path);
    TEST_CHECK(cascade == NULL);

    free_cascade(cascade);

    remove(file_path);
}