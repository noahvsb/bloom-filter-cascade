#include "arguments_parser.h"

Command help_message(char* program) {
    printf("Usage: %s [train|classify] <arguments>\n\n", program);
    printf("train:\n");
    printf("  Arguments:\n");
    printf("    -o <file>    output file\n");
    printf("    <file>       input file (last argument that isn't preceded by a flag)\n");
    printf("  Examples:\n");
    printf("    %s train -o Bloom_filters.file categories.file\n", program);
    printf("    %s train categories.file -o Bloom_filters.file\n\n", program);
    printf("classify:\n");
    printf("  Arguments:\n");
    printf("    <file>       input file\n");
    printf("  Example:\n");
    printf("    %s classify Bloom_filters.file\n", program);
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
                return help_message(argv[0]);
        } else
            train.inputFile = argv[i];
        i++;
    }

    if (strlen(train.outputFile) == 0 || strlen(train.inputFile) == 0)
        return help_message(argv[0]);

    Command command = { TRAIN, { .train = train } };
    return command;
}

Command parse_classify(int argc, char** argv) {
    Classify classify = { argv[2] };
    Command command = { CLASSIFY, { .classify = classify } };
    return command;
}

Command parse_arguments(int argc, char** argv) {
    // train
    if (argc > 4 && strcmp(argv[1], "train") == 0)
        return parse_train(argc, argv);

    // classify
    if (argc > 2 && strcmp(argv[1], "classify") == 0)
        return parse_classify(argc, argv);

    return help_message(argv[0]);
}

