#define TEST_NO_MAIN

#include "acutest.h"
#include "../src/arguments/arguments_parser.h"

#define PROGRAM "./test_bloom_filter"

void test_help_message(void) {
    int argc = 2;
    char* argv[] = { PROGRAM, "--help" };
    
    printf("\n");
    Command command = parse_arguments(argc, argv);

    TEST_CHECK(command.type == NONE);
}

void test_train_command(void) {
    int argc = 5;
    char* argv[] = { PROGRAM, "train", "-o", "cascade.bfc", "categories.txt" };

    Command command = parse_arguments(argc, argv);

    TEST_CHECK(command.type == TRAIN);
    TEST_CHECK(strcmp(command.data.train.outputFile, argv[3]) == 0);
    TEST_CHECK(strcmp(command.data.train.inputFile, argv[4]) == 0);
}

void test_classify_command(void) {
    int argc = 3;
    char* argv[] = { PROGRAM, "classify", "cascade.bfc" };

    Command command = parse_arguments(argc, argv);

    TEST_CHECK(command.type == CLASSIFY);
    TEST_CHECK(strcmp(command.data.classify.inputFile, argv[2]) == 0);
}