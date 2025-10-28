#include "cascade_parser.h"

static void* read_error(const char* file_path, Cascade* cascade, FILE* file) {
    fprintf(stderr, "Failed to read from file: %s\n", file_path);
    return clean_return(2, cascade, free_cascade, file, fclose);
}

Cascade* parse_cascade(char* file_path) {
    Cascade* cascade = malloc(sizeof(Cascade));
    if (!cascade) {
        fprintf(stderr, "Failed to allocate cascade\n");
        return NULL;
    }

    FILE* file = fopen(file_path, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", file_path);
        return clean_return(1, cascade, free_cascade);
    }

    size_t read_size;
    
    // parse categories names
    uint32_t category_size;
    read_size = fread(&category_size, sizeof(uint32_t), 1, file);
    if (read_size != 1) return read_error(file_path, cascade, file);

    cascade->categories_size = category_size;
    cascade->categories_names = malloc(sizeof(char*) * category_size);
    if (!cascade->categories_names) {
        fprintf(stderr, "Failed to allocate categories names\n");
        return clean_return(2, cascade, free_cascade, file, fclose);
    }

    for (uint32_t i = 0; i < category_size; i++) {
        uint8_t length;
        read_size = fread(&length, sizeof(uint8_t), 1, file);
        if (read_size != 1) return read_error(file_path, cascade, file);

        char* name = malloc(sizeof(char) * (length + 1));
        read_size = fread(name, sizeof(char), length, file);
        if (read_size != length) return read_error(file_path, cascade, file);
        name[length] = '\0';

        cascade->categories_names[i] = name;
    }

    // parse bloomfilters

    // parse last categories names

    return cascade;
}

void free_cascade(Cascade* cascade) {
    if (cascade) {
        if (cascade->categories_names) {
            for (uint32_t i = 0; i < cascade->categories_size; i++)
                if (cascade->categories_names[i])
                    free(cascade->categories_names[i]);
            free(cascade->categories_names);
        }
        if (cascade->last_category_name)
            free(cascade->last_category_name);
        free(cascade);
    }
}