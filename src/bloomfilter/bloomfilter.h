#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../categories/categories_parser.h"
#include "../hash/murmurhash.h"

uint64_t* create_bloomfilter(CategoryList* list, uint64_t except); // creates a bloomfilter with all categories from list except the one at index = except

#endif