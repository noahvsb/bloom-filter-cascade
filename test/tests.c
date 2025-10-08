#include "tests.h"

TEST_LIST = {
    { "args: help message", testHelpMessage },
    { "args: train command", testTrainCommand },
    { "args: classify command", testClassifyCommand },
    { NULL, NULL }
};