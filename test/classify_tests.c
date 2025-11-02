#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/classify/classify.h"
#include "../src/file/categories_parser.h"
#include "../src/file/cascade_parser.h"

void test_classify_template(char* categories_file_path, char* cascade_file_path) {
    CategoryList* list = parse_categories(categories_file_path);
    Cascade* cascade = parse_cascade(cascade_file_path);

    for (uint32_t i = 0; i < list->categories_size; i++) {
        Category* category = list->categories[i];
        char* category_name = category->name;
        for (uint32_t j = 0; j < category->size; j++) {
            char* result = classify(cascade, category->elements[j]);

            TEST_CHECK(strcmp(category_name, result) == 0);
        }
    }

    free_categories(list);
    free_cascade(cascade);
}

void test_classify_large(void) {
    test_classify_template("data/large.txt", "data/large.bfc");
}

void test_classify(void) {
    test_classify_template("test/data/test.txt", "test/data/test.bfc");
}