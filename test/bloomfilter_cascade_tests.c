#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/bloomfilter/bloomfilter.h"

void test_bloomfilter_cascade_create_fast(void) {
    char* input_file_path = "test/data/test.txt";
    char* output_file_path = "test/data/temp_fast.bfc";
    CategoryList* list = parse_categories(input_file_path);

    printf("\n");
    uint8_t status = create_bloomfilter_cascade(output_file_path, list, 0, 3);
    TEST_CHECK(status == 0);

    // not checking the content of the cascade file:
    // - the file writing is tested in cascade_writer_tests.c
    // - the correctness will be tested in classify_test.c

    free_categories(list);
    remove(output_file_path);
}

void test_bloomfilter_cascade_create_less_storage(void) {
    char* input_file_path = "test/data/test.txt";
    char* output_file_path = "test/data/temp_less_storage.bfc";
    CategoryList* list = parse_categories(input_file_path);

    printf("\n");
    uint8_t status = create_bloomfilter_cascade(output_file_path, list, 1, 3);
    TEST_CHECK(status == 0);

    // not checking the content of the cascade file:
    // - the file writing is tested in cascade_writer_tests.c
    // - the correctness will be tested in classify_test.c

    free_categories(list);
    remove(output_file_path);
}
