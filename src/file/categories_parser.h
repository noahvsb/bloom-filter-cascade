#ifndef CATEGORIES_PARSER_H
#define CATEGORIES_PARSER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct Category {
    char* name;
    char** elements;
    uint32_t size;
    uint32_t leftover_size; // size of memory that has been allocated, but not yet initialized
} Category;

typedef struct CategoryList {
    Category** categories;
    uint32_t categories_size;
    uint32_t leftover_size; // size of memory that has been allocated, but not yet initialized
    uint32_t elements_size;
} CategoryList;

#include "../tools/clean_return.h"

CategoryList* parse_categories(char* file_path);

void free_category(Category* category);
void free_categories(CategoryList* list);

#endif