#include "tests.h"

TEST_LIST = {
    { "args: help message", test_help_message },
    { "args: train command", test_train_command },
    { "args: classify command", test_classify_command },

    { "categories: parse", test_parse_categories },
    { "categories: parse failcheck", test_parse_categories_failcheck },

    { "hash: murmurhash", test_murmurhash },

    { "bloomfilter: create", test_bloomfilter_create },
    { "bloomfilter: create failcheck", test_bloomfilter_create_failcheck },

    { "writer: start", test_write_start },
    { "writer: bloomfilter", test_write_bloomfilter },
    { "writer: end", test_write_end },

    { NULL, NULL }
};