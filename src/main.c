#include <stdio.h>
#include "arguments/arguments_parser.h"
#include "file/categories_parser.h"
#include "file/cascade_parser.h"
#include "bloomfilter/bloomfilter.h"
#include "classify/classify.h"

int main(int argc, char** argv) {
    srand((unsigned) time(NULL));

    Command command = parse_arguments(argc, argv);
    switch (command.type) {
        case TRAIN:
            CategoryList* list = parse_categories(command.data.train.inputFile);
            if (!list) exit(1);
            printf("Amount of elements in category list: %d\n\n", list->elements_size);
            uint8_t status = create_bloomfilter_cascade(command.data.train.outputFile, list, command.data.train.algorithm, 5);
            free_categories(list);
            exit(status);
        case CLASSIFY:
            Cascade* cascade = parse_cascade(command.data.classify.inputFile);
            if (!cascade) exit(1);
            run_classify(cascade);
            free_cascade(cascade);
            break;
        default:
            exit(1);
    }
    exit(0);
}