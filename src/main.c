#include <stdio.h>
#include "arguments/arguments_parser.h"
#include "categories/categories_parser.h"
#include "bloomfilter/bloomfilter.h"

int main(int argc, char** argv) {
    Command command = parse_arguments(argc, argv);
    switch (command.type) {
        case TRAIN:
            printf("input: %s, output: %s\n", command.data.train.inputFile, command.data.train.outputFile);
            CategoryList* list = parse_categories(command.data.train.inputFile);
            if (!list) exit(1);
            printf("Amount of elements in category list: %ld\n", list->elements_size);
            uint64_t* bloomfilter = create_bloomfilter(list, 0);
            if (!bloomfilter) exit(1);
            // TODO
            for (int i = 63; i >= 0; i--) {
                putchar((bloomfilter[0] & (1ULL << i)) ? '1' : '0');
            }
            putchar('\n');
            free_categories(list);
            free(bloomfilter);
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