#include "tests.h"

TEST_LIST = {
    { "args: help message", testHelpMessage },
    { "args: train command", testTrainCommand },
    { "args: classify command", testClassifyCommand },
    { "categories: parse", testParseCategories },
    { "categories: failcheck", testParseCategoriesFailcheck },
    { "hash: murmurhash", testMurmurhash },
    { "bloomfilter: create", testBloomfilterCreate },
    { "bloomfilter: failcheck", testBloomfilterFailcheck },
    { NULL, NULL }
};