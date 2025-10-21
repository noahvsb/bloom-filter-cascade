#ifndef BLOOMFILTER_CASCADE_H
#define BLOOMFILTER_CASCADE_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../file/categories_parser.h"

/**
 * returns 0 if success
 * returns 1 if fail
*/
uint8_t create_bloomfilter_cascade(CategoryList* list, char* file_path);

#endif

