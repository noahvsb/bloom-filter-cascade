#include "cascade_writer.h"

FILE* write_start(char* file_path, CategoryList* list, bool algorithm) {
    FILE* file = fopen(file_path, "wb");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", file_path);
        return NULL;
    }

    fwrite(&(algorithm), sizeof(bool), 1, file);

    fwrite(&(list->categories_size), sizeof(uint32_t), 1, file);

    for (uint32_t i = 0; i < list->categories_size; i++) {
        char* category_name = list->categories[i]->name;
        uint8_t len = strlen(category_name);
        fwrite(&len, sizeof(uint8_t), 1, file);
        fwrite(category_name, sizeof(char), len, file);
    }

    return file;
}

void write_bloomfilter(Bloomfilter* bloomfilter, FILE* file) {
    fwrite(&(bloomfilter->hash_amount), sizeof(uint8_t), 1, file);
    fwrite(bloomfilter->hash_seeds, sizeof(uint8_t), bloomfilter->hash_amount, file);
    fwrite(&(bloomfilter->size), sizeof(uint32_t), 1, file);
    fwrite(bloomfilter->bf, sizeof(uint8_t), bloomfilter->size, file);
}

void write_end(char* category_name, FILE* file) {
    fwrite(&(uint8_t){0xFF}, sizeof(uint8_t), 1, file);
    uint8_t len = strlen(category_name);
    fwrite(&len, sizeof(uint8_t), 1, file);
    fwrite(category_name, sizeof(char), len, file);
    fclose(file);
}



