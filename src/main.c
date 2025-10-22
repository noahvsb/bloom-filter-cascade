#include <stdio.h>
#include "arguments/arguments_parser.h"
#include "file/categories_parser.h"
#include "bloomfilter/bloomfilter_cascade.h"

int main(int argc, char** argv) {
    srand((unsigned) time(NULL));

    Command command = parse_arguments(argc, argv);
    switch (command.type) {
        case TRAIN:
            CategoryList* list = parse_categories(command.data.train.inputFile);
            if (!list) exit(1);
            printf("Amount of elements in category list: %d\n", list->elements_size);
            uint8_t status = create_bloomfilter_cascade(list, command.data.train.outputFile, 5);
            free_categories(list);
            if (status == 0) printf("Succesfully wrote bloomfilter cascade to: %s\n", command.data.train.outputFile);
            exit(status);
        case CLASSIFY:
            // TODO
            printf("input: %s\n", command.data.classify.inputFile);
            break;
        default:
            exit(1);
    }
    exit(0);
}