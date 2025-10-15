#ifndef ARGUMENTS_PARSER_H
#define ARGUMENTS_PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct Train {
    char* outputFile;
    char* inputFile;
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

Command parse_arguments(uint8_t argc, char** argv);

#endif