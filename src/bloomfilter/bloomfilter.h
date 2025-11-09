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
 * @param except: the category that should be left out of the bloomfilter, do except < 0 if you don't want to exclude a category
 * @param only: the only category that should be included in the bloomfilter, do only < 0 if you want multiple categories
 *     If both except and only are >= 0, except gets priority, so only is treated as < 0
 * @param k: the chance for false positives will be around 1 / 2^k (with k > 0)
 */
Bloomfilter* create_bloomfilter(CategoryList* list, int32_t except, int32_t only, uint8_t k);

bool test_bloomfilter(Bloomfilter* bloomfilter, char* element_name);

/**
 * creates a bloomfilter cascade, which it writes to file_path
 * 
 * @param list: category list of all categories
 * @param file_path: file path to write the cascade to
 * @param k: same as k in create_bloomfilter()
 * 
 * @returns 0 if success
 * @returns 1 if fail
*/
uint8_t create_bloomfilter_cascade(char* file_path, CategoryList* list, bool algorithm, uint8_t k);

void free_bloomfilter(Bloomfilter* bloomfilter);

#endif