#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/classify/classify.h"
#include "../src/file/categories_parser.h"
#include "../src/file/cascade_parser.h"
#include <stdbool.h>

void test_classify_template(char* categories_file_path, char* cascade_file_path, bool print) {
    if (print) { printf("\nParsing"); fflush(stdout); }
    CategoryList* list = parse_categories(categories_file_path);
    Cascade* cascade = parse_cascade(cascade_file_path);
    if (print) printf("    OK\n");

    for (uint32_t i = 0; i < list->categories_size; i++) {
        Category* category = list->categories[i];
        if (print) { printf("Testing %s", category->name); fflush(stdout); }
        for (uint32_t j = 0; j < category->size; j++) {
            char* result = classify(cascade, category->elements[j]);

            TEST_CHECK(strcmp(category->name, result) == 0);
        }
        if (print) printf("    OK\n");
    }

    free_categories(list);
    free_cascade(cascade);
}

void test_classify_large(void) {
    test_classify_template("data/large.txt", "data/large.bfc", true);
}

void test_classify_simple(void) {
    test_classify_template("test/data/test.txt", "test/data/test0.bfc", false);
}

// fun fact: the enhanced performs worse in this case, because test/data/test.txt is very small
void test_classify_enhanced(void) {
    test_classify_template("test/data/test.txt", "test/data/test1.bfc", false);
}

