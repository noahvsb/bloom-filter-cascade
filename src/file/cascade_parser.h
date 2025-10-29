#ifndef CASCADE_PARSER_H
#define CASCADE_PARSER_H

#include <stdlib.h>
#include <stdint.h>
#include "../bloomfilter/bloomfilter.h"
#include "../tools/clean_return.h"

typedef struct Cascade {
    uint32_t categories_size;
    char** categories_names;
    uint32_t steps;
    Bloomfilter** bloomfilters;
    char* last_category_name;
} Cascade;

Cascade* parse_cascade(char* file_path);

void free_cascade(Cascade* cascade);

#endif