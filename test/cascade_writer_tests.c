#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/file/cascade_writer.h"

void test_write_start(void) {
    char* input_file_path = "test/data/test.txt";
    char* output_file_path = "test/data/temp.bfc";
    CategoryList* list = parse_categories(input_file_path);

    bool algorithm = 1;

    FILE* file = write_start(output_file_path, list, algorithm);
    fclose(file);

    file = fopen(output_file_path, "rb");
    if (!file) {
        fprintf(stderr, "Error opening output file for checking correctness\n");
        return;
    }

    size_t read_size;

    bool algorithm_output;
    read_size = fread(&algorithm_output, sizeof(bool), 1, file);
    TEST_CHECK(read_size == 1);
    TEST_CHECK(algorithm_output == algorithm);

    uint32_t categories_size;
    read_size = fread(&categories_size, sizeof(uint32_t), 1, file);
    TEST_CHECK(read_size == 1);
    TEST_CHECK(categories_size == 5);

    for (uint32_t i = 0; i < categories_size; i++) {
        uint8_t length;
        read_size = fread(&length, sizeof(uint8_t), 1, file);
        TEST_CHECK(read_size == 1);
        TEST_CHECK(length == 9);

        char name[32];
        read_size = fread(name, sizeof(char), length, file);
        TEST_CHECK(read_size == length);
        name[length] = '\0';

        char expected[32];
        sprintf(expected, "category%u", i);

        TEST_CHECK(strcmp(name, expected) == 0);
    }

    fclose(file);
    free_categories(list);
    remove(output_file_path);
}

void test_write_bloomfilter(void) {
    char* input_file_path = "test/data/test.txt";
    char* output_file_path = "test/data/temp.bfc";
    CategoryList* list = parse_categories(input_file_path);
    Bloomfilter* bloomfilter = create_bloomfilter(list, -1, -1, 8);

    FILE* file = fopen(output_file_path, "wb");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", output_file_path);
        return;
    }
    write_bloomfilter(bloomfilter, file);
    fclose(file);

    file = fopen(output_file_path, "rb");
    if (!file) {
        fprintf(stderr, "Error opening output file for checking correctness\n");
        return;
    }

    size_t read_size;

    uint8_t hash_amount;
    read_size = fread(&hash_amount, sizeof(uint8_t), 1, file);
    TEST_CHECK(read_size == 1);
    TEST_CHECK(hash_amount == bloomfilter->hash_amount);

    for (uint8_t i = 0; i < hash_amount; i++) {
        uint8_t hash_seed;
        read_size = fread(&hash_seed, sizeof(uint8_t), 1, file);
        TEST_CHECK(read_size == 1);
        TEST_CHECK(hash_seed == bloomfilter->hash_seeds[i]);
    }

    uint32_t size;
    read_size = fread(&size, sizeof(uint32_t), 1, file);
    TEST_CHECK(read_size == 1);
    TEST_CHECK(size == bloomfilter->size);

    for (uint8_t i = 0; i < size; i++) {
        uint8_t bf_part;
        read_size = fread(&bf_part, sizeof(uint8_t), 1, file);
        TEST_CHECK(read_size == 1);
        TEST_CHECK(bf_part == bloomfilter->bf[i]);
    }

    fclose(file);
    free_categories(list);
    free_bloomfilter(bloomfilter);
    remove(output_file_path);
}

void test_write_end(void) {
    char* input_file_path = "test/data/test.txt";
    char* output_file_path = "test/data/temp.bfc";
    CategoryList* list = parse_categories(input_file_path);

    FILE* file = fopen(output_file_path, "wb");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", output_file_path);
        return;
    }
    write_end(list->categories[0]->name, file);

    file = fopen(output_file_path, "rb");
    if (!file) {
        fprintf(stderr, "Error opening output file for checking correctness\n");
        return;
    }

    size_t read_size;

    uint8_t end_bytes;
    read_size = fread(&end_bytes, sizeof(uint8_t), 1, file);
    TEST_CHECK(read_size == 1);
    TEST_CHECK(end_bytes == 0xFF);

    uint8_t name_len;
    read_size = fread(&name_len, sizeof(uint8_t), 1, file);
    TEST_CHECK(read_size == 1);
    TEST_CHECK(name_len == strlen(list->categories[0]->name));

    char name[256];
    read_size = fread(name, sizeof(char), name_len, file);
    TEST_CHECK(read_size == name_len);
    name[name_len] = '\0';

    TEST_CHECK(strcmp(name, "category0") == 0);

    fclose(file);
    free_categories(list);
    remove(output_file_path);
}
