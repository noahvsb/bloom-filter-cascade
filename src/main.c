#include <stdio.h>
#include "arguments_parser.h"
#include "categories_parser.h"

int main(int argc, char** argv) {
    Command command = parse_arguments(argc, argv);
    switch (command.type) {
        case TRAIN:
            printf("input: %s, output: %s\n", command.data.train.inputFile, command.data.train.outputFile);
            CategoryList* list = parse_categories(command.data.train.inputFile);
            if (!list) exit(1);
            // TODO
            for (uint8_t i = 0; i < list->size; i++) {
                printf("%s with size %ld and first element %s\n", list->items[i]->name, list->items[i]->size, list->items[i]->elements[0]);
            }
            free_categories(list);
            break;
        case CLASSIFY:
            // TODO
            printf("input: %s\n", command.data.classify.inputFile);
            break;
        default:
            exit(1);
    }
    exit(0);
}