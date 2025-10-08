#include "arguments_parser.h"

Command help_message(char* program) {
    printf("TODO: implement %s --help\n", program);
    return (Command) { NONE };
}

Command usage_error(char* program) {
    fprintf(stderr, "Usage: %s [train|classify] [...] (see %s --help for extra necessary arguments)\n", program, program);
    return (Command) { NONE };
}

Command parse_train(int argc, char** argv) {
    Train train = { "", "" };

    int i = 2;
    while (i < argc) {
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            i++;
            if (i < argc)
                train.outputFile = argv[i];
            else 
                return usage_error(argv[0]);
        } else
            train.inputFile = argv[i];
        i++;
    }

    if (strlen(train.outputFile) == 0 || strlen(train.inputFile) == 0)
        return usage_error(argv[0]);

    Command command = { TRAIN, { .train = train } };
    return command;
}

Command parse_classify(int argc, char** argv) {
    Classify classify = { argv[2] };
    Command command = { CLASSIFY, { .classify = classify } };
    return command;
}

Command parse_arguments(int argc, char** argv) {
    if (argc < 2)
        return usage_error(argv[0]);
    
    // help flag
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        return help_message(argv[0]);

    // train
    if (strcmp(argv[1], "train") == 0 && argc > 4)
        return parse_train(argc, argv);

    // classify
    if (strcmp(argv[1], "classify") == 0 && argc > 2)
        return parse_classify(argc, argv);

    return usage_error(argv[0]);
}

