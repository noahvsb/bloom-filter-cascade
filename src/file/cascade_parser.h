#ifndef CASCADE_PARSER_H
#define CASCADE_PARSER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../bloomfilter/bloomfilter.h"
#include "../tools/clean_return.h"

typedef struct FastCascade {
    uint32_t categories_size;
    char** categories_names;
    uint32_t bloomfilters_size;
    Bloomfilter** bloomfilters;
    char* last_category_name;
} FastCascade;

typedef struct LessStorageCascade {
    uint32_t categories_size;
    char** categories_names;
    // TODO
} LessStorageCascade;

typedef struct Cascade {
    bool algorithm;
    FastCascade* fast;
    LessStorageCascade* less_storage;
} Cascade;

Cascade* parse_cascade(char* file_path);

void free_cascade(Cascade* cascade);

#endif