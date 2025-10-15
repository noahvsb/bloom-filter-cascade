#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/categories/categories_parser.h"

void testParseCategories(void) {
    char* file_path = "test/data/test.txt";

    CategoryList* list = parse_categories(file_path);

    TEST_CHECK(list->size == 5);
    TEST_CHECK(list->leftover_size == 5);
    
    Category* category0 = list->items[0];
    Category* category1 = list->items[1];
    Category* category2 = list->items[2];
    Category* category3 = list->items[3];
    Category* category4 = list->items[4];

    TEST_CHECK(category0->size == 3);
    TEST_CHECK(category0->leftover_size == 7);
    TEST_CHECK(category1->size == 1);
    TEST_CHECK(category1->leftover_size == 9);
    TEST_CHECK(category2->size == 0);
    TEST_CHECK(category2->leftover_size == 10);
    TEST_CHECK(category3->size == 2);
    TEST_CHECK(category3->leftover_size == 8);
    TEST_CHECK(category4->size == 11);
    TEST_CHECK(category4->leftover_size == 9);

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
}

void testParseCategoriesFail(void) {
    char* file_path = "test/data/unknown.txt";
    printf("\n");
    CategoryList* list = parse_categories(file_path);
    TEST_CHECK(list == NULL);
    
    file_path = "test/data/testfail.txt";
    list = parse_categories(file_path);
    TEST_CHECK(list == NULL);
}

