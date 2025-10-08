#include <stdio.h>
#include "arguments_parser.h"

int main(int argc, char** argv) {
    Command command = parse_arguments(argc, argv);
    switch (command.type) {
        case TRAIN:
            printf("input: %s, output: %s\n", command.data.train.inputFile, command.data.train.outputFile);
            break;
        case CLASSIFY:
            printf("input: %s\n", command.data.classify.inputFile);
            break;
        default:
            exit(1);
    }
    exit(0);
}