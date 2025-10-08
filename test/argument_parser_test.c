#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/arguments_parser.h"

#define COMMAND "./test_bloom_filter"

void testUsageError(void) {
    int argc = 3;
    char* argv[] = { COMMAND, "train", "error" };
    
    printf("\n");
    Command command = parse_arguments(argc, argv);

    TEST_CHECK(command.type == NONE);
}

void testHelpMessage(void) {
    int argc = 2;
    char* argv[] = { COMMAND, "--help" };
    
    printf("\n");
    Command command = parse_arguments(argc, argv);

    TEST_CHECK(command.type == NONE);
}

void testTrainCommand(void) {
    int argc = 5;
    char* argv[] = { COMMAND, "train", "-o", "cascade.file", "categories.file" };

    Command command = parse_arguments(argc, argv);

    TEST_CHECK(command.type == TRAIN);
    TEST_CHECK(strcmp(command.data.train.outputFile, argv[3]) == 0);
    TEST_CHECK(strcmp(command.data.train.inputFile, argv[4]) == 0);
}

void testClassifyCommand(void) {
    int argc = 3;
    char* argv[] = { COMMAND, "classify", "cascade.file" };

    Command command = parse_arguments(argc, argv);

    TEST_CHECK(command.type == CLASSIFY);
    TEST_CHECK(strcmp(command.data.classify.inputFile, argv[2]) == 0);
}