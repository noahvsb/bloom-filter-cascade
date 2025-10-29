#ifndef CASCADE_WRITER_H
#define CASCADe_WRITER_H

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "categories_parser.h"
#include "../bloomfilter/bloomfilter.h"

FILE* write_start(CategoryList* list, char* file_path);

void write_bloomfilter(Bloomfilter* bloomfilter, FILE* file);

/**
 * @param category: name of the only non-empty category left
 */
void write_end(char* category_name, FILE* file);

#endif
