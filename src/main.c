#include <stdio.h>
#include "arguments/arguments_parser.h"
#include "file/categories_parser.h"
#include "file/cascade_parser.h"
#include "bloomfilter/bloomfilter.h"

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
            exit(status);
        case CLASSIFY:
            Cascade* cascade = parse_cascade(command.data.classify.inputFile);
            printf("Amount of categories in cascade: %d\n", cascade->categories_size);
            for (uint32_t i = 0; i < cascade->categories_size; i++) {
                printf("Category %d: %s\n", i, cascade->categories_names[i]);
            }
            printf("Amount of steps in cascade: %d\n", cascade->cascade_steps);
            for (uint32_t i = 0; i < cascade->categories_size * cascade->cascade_steps; i++) {
                printf("bloomfilter %d size: %d\n", i, cascade->bloomfilters[i] ? cascade->bloomfilters[i]->size * 8 : 0);
            }
            printf("Last category name: %s\n", cascade->last_category_name ? cascade->last_category_name : "NULL");
            free(cascade);
            break;
        default:
            exit(1);
    }
    exit(0);
}