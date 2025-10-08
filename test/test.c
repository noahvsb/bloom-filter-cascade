#include "test.h"

TEST_LIST = {
    { "args: usage error", testUsageError },
    { "args: help message", testHelpMessage },
    { "args: train command", testTrainCommand },
    { "args: classify command", testClassifyCommand },
    { NULL, NULL }
};