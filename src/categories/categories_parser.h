#ifndef CATEGORIES_PARSER_H
#define CATEGORIES_PARSER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct Category {
    char* name;
    char** elements;
    uint64_t size;
    uint64_t leftover_size; // size of memory that has been allocated, but not yet initialized
} Category;

typedef struct CategoryList {
    Category** items;
    uint64_t size;
    uint64_t leftover_size; // size of memory that has been allocated, but not yet initialized
} CategoryList;

CategoryList* parse_categories(char* file_path);

void free_categories(CategoryList* list);

#endif