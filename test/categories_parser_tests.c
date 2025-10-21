#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/file/categories_parser.h"

void testParseCategories(void) {
    char* file_path = "test/data/test.txt";

    CategoryList* list = parse_categories(file_path);

    TEST_CHECK(list->categories_size == 5);
    TEST_CHECK(list->leftover_size == 27);
    TEST_CHECK(list->elements_size == 39);
    
    Category* category0 = list->categories[0];
    Category* category1 = list->categories[1];
    Category* category2 = list->categories[2];
    Category* category3 = list->categories[3];
    Category* category4 = list->categories[4];

    TEST_CHECK(category0->size == 3);
    TEST_CHECK(category0->leftover_size == 29);
    TEST_CHECK(category1->size == 1);
    TEST_CHECK(category1->leftover_size == 31);
    TEST_CHECK(category2->size == 0);
    TEST_CHECK(category2->leftover_size == 32);
    TEST_CHECK(category3->size == 2);
    TEST_CHECK(category3->leftover_size == 30);
    TEST_CHECK(category4->size == 33);
    TEST_CHECK(category4->leftover_size == 31);

    TEST_CHECK(strcmp(category0->name, "category0") == 0);
    TEST_CHECK(strcmp(category1->name, "category1") == 0);
    TEST_CHECK(strcmp(category2->name, "category2") == 0);
    TEST_CHECK(strcmp(category3->name, "category3") == 0);
    TEST_CHECK(strcmp(category4->name, "category4") == 0);

    TEST_CHECK(strcmp(category0->elements[0], "element0") == 0);
    TEST_CHECK(strcmp(category0->elements[1], "element1") == 0);
    TEST_CHECK(strcmp(category0->elements[2], "element2") == 0);
    TEST_CHECK(strcmp(category1->elements[0], "element3") == 0);
    TEST_CHECK(strcmp(category3->elements[0], "element4") == 0);
    TEST_CHECK(strcmp(category3->elements[1], "element5") == 0);
    // not checking all category4 elements, category4 was just there to check if leftover_size gets updated if it becomes 0

    free_categories(list);
}

void testParseCategoriesFailcheck(void) {
    char* file_path = "test/data/unknown.txt";
    printf("\n");
    CategoryList* list = parse_categories(file_path);
    TEST_CHECK(list == NULL);

    free_categories(list);
    
    file_path = "test/data/testfail.txt";
    list = parse_categories(file_path);
    TEST_CHECK(list == NULL);

    free_categories(list);
}

