#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/bloomfilter/bloomfilter.h"

void test_bloomfilter_cascade_create_fast(void) {
    srand(123);

    char* input_file_path = "test/data/test.txt";
    char* output_file_path = "test/data/temp_fast.bfc";
    CategoryList* list = parse_categories(input_file_path);

    printf("\n");
    uint8_t status = create_bloomfilter_cascade(output_file_path, list, 0);
    TEST_CHECK(status == 0);

    // not checking the content of the cascade file:
    // - the file writing is tested in cascade_writer_tests.c
    // - the correctness will be tested in classify_test.c

    free_categories(list);
    remove(output_file_path);
}

void test_bloomfilter_cascade_create_less_storage(void) {
    srand(123);
    
    char* input_file_path = "test/data/test.txt";
    char* output_file_path = "test/data/temp_less_storage.bfc";
    CategoryList* list = parse_categories(input_file_path);

    printf("\n");
    uint8_t status = create_bloomfilter_cascade(output_file_path, list, 1);
    TEST_CHECK(status == 0);

    // not checking the content of the cascade file:
    // - the file writing is tested in cascade_writer_tests.c
    // - the correctness will be tested in classify_test.c

    free_categories(list);
    remove(output_file_path);
}

void test_bloomfilter_cascade_failcheck(void) {
    srand(123);

    // create empty file
    char* input_file_path = "test/data/temp_empty.txt";
    fclose(fopen(input_file_path, "wb"));
    char* output_file_path = "test/data/temp_empty.bfc";

    CategoryList* list = parse_categories(input_file_path);

    printf("\n");
    uint8_t status = create_bloomfilter_cascade(output_file_path, list, 1);
    TEST_CHECK(status == 0);

    free_categories(list);
    remove(input_file_path); remove(output_file_path);
}
