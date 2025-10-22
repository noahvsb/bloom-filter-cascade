#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../file/categories_parser.h"
#include "../hash/murmurhash.h"
#include "../tools/clean_return.h"
#include <time.h>

typedef struct Bloomfilter {
    uint32_t size; // amount of bits / 8
    uint8_t hash_amount;
    uint8_t* bf;
    uint8_t* hash_seeds;
} Bloomfilter;

/**
 * creates a bloomfilter with all categories from list except the one at index = except
 * 
 * @param list: category list of all categories
 * @param except: the category that should be left out of the bloomfilter, put -1 if you don't want to exclude a category
 * @param p: the chance for false positives will be lower or equal than 1 / 2^p (with p > 1)
 */
Bloomfilter* create_bloomfilter(CategoryList* list, int32_t except, uint8_t p);

void free_bloomfilter(Bloomfilter* bloomfilter);

#endif