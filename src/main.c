#include <stdio.h>
#include "arguments_parser.h"

int main(int argc, char** argv) {
    Command command = parse_arguments(argc, argv);
    if (command.type == NONE) exit(1);
    exit(0);
}