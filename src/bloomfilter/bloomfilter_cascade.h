#ifndef BLOOMFILTER_CASCADE_H
#define BLOOMFILTER_CASCADE_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../file/categories_parser.h"
#include "../file/cascade_writer.h"
#include "../tools/clean_return.h"
#include "../hash/murmurhash.h"

/**
 * returns 0 if success
 * returns 1 if fail
*/
uint8_t create_bloomfilter_cascade(CategoryList* list, char* file_path, uint8_t p);

#endif

