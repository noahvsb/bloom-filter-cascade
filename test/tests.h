#ifndef TESTS_H
#define TESTS_H

#include "acutest.h"

// argument parser tests
void testHelpMessage(void);
void testTrainCommand(void);
void testClassifyCommand(void);

// category parser tests
void testParseCategories(void);
void testParseCategoriesFail(void);

// hash tests
void testMurmurhash(void); // simple test to see if I copy pasted correctly ;)

#endif