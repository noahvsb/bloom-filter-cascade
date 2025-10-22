#ifndef TESTS_H
#define TESTS_H

#include "acutest.h"

// argument parser tests
void test_help_message(void);
void test_train_command(void);
void test_classify_command(void);

// category parser tests
void test_parse_categories(void);
void test_parse_categories_failcheck(void);

// hash tests
void test_murmurhash(void); // simple test to see if I copy pasted correctly ;)

// bloomfilter tests
void test_bloomfilter_create(void);
void test_bloomfilter_create_failcheck(void);

// cascade writer tests
void test_write_start(void);
void test_write_bloomfilter(void);
void test_write_end(void);

#endif