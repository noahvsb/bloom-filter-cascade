#include "bloomfilter_cascade.h"

uint8_t create_bloomfilter_cascade(CategoryList* list, char* file_path) {
    FILE* file = write_start(list, file_path);
    if (!file) return 1;

    while (list->categories_size > 1) {
        list->categories_size--;
    }

    write_end(list->categories_size != 0 ? list->categories[0]->name : "", file);
    return 0;
}