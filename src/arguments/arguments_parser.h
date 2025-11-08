#ifndef ARGUMENTS_PARSER_H
#define ARGUMENTS_PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Train {
    char* outputFile;
    char* inputFile;
    bool algorithm;
} Train;

typedef struct Classify {
    char* inputFile;
} Classify;

typedef enum CommandType {
    NONE, TRAIN, CLASSIFY
} CommandType;

typedef union CommandData {
    Train train;
    Classify classify;
} CommandData;

typedef struct Command {
    CommandType type;
    CommandData data;
} Command;

Command parse_arguments(int argc, char** argv);

#endif