#include "arguments_parser.h"

void print_help(char* program) {
    printf("TODO: implement %s --help\n", program);
}

void print_usage_error(char* program) {
    fprintf(stderr, "Usage: %s [train|classify] [...] (see %s --help for extra necessary arguments)\n", program, program);
}

Command parse_train(int argc, char** argv) {
    Command train = { TRAIN };

    // TODO

    return train;
}

Command parse_classify(int argc, char** argv) {
    Command classify = { CLASSIFY };

    // TODO

    return classify;
}

Command parse_arguments(int argc, char** argv) {
    Command none = { NONE };

    if (argc < 2) {
        print_usage_error(argv[0]);
        return none;
    }
    
    // help flag
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_help(argv[0]);
        return none;
    }

    // train
    if (strcmp(argv[1], "train") == 0) {
        return parse_train(argc, argv);
    }

    // classify
    if (strcmp(argv[1], "classify") == 0) {
        return parse_classify(argc, argv);
    }

    print_usage_error(argv[0]);
    return none;
}

