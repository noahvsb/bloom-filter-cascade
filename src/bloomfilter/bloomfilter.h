#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

typedef struct Bloomfilter {
    uint32_t size; // amount of bits / 8
    uint8_t hash_amount;
    uint8_t* bf;
    uint8_t* hash_seeds;
} Bloomfilter;

#include "../file/categories_parser.h"
#include "../file/cascade_writer.h"
#include "../hash/murmurhash.h"
#include "../tools/clean_return.h"

/**
 * creates a bloomfilter with all categories from list except the one at index = except
 * 
 * @param list: category list of all categories
 * @param except: the category that should be left out of the bloomfilter, put -1 if you don't want to exclude a category
 * @param p: the chance for false positives will be lower or equal than 1 / 2^p (with p > 1)
 */
Bloomfilter* create_bloomfilter(CategoryList* list, int32_t except, uint8_t p);

/**
 * creates a bloomfilter cascade, which it writes to file_path
 * 
 * @param list: category list of all categories
 * @param file_path: file path to write the cascade to
 * @param p: same as p in create_bloomfilter()
 * 
 * @returns 0 if success
 * @returns 1 if fail
*/
uint8_t create_bloomfilter_cascade(CategoryList* list, char* file_path, uint8_t p);

void free_bloomfilter(Bloomfilter* bloomfilter);

#endif