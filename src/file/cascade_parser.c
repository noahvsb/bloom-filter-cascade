#include "cascade_parser.h"

static void* read_error(char* msg, char* file_path, Cascade* cascade, FILE* file, Bloomfilter* bloomfilter) {
    fprintf(stderr, "Failed to read %s from file: %s\n", msg, file_path);
    return clean_return(3, cascade, free_cascade, file, fclose, bloomfilter, free_bloomfilter);
}

Cascade* parse_cascade(char* file_path) {
    Cascade* cascade = malloc(sizeof(Cascade));
    if (!cascade) {
        fprintf(stderr, "Failed to allocate cascade\n");
        return NULL;
    }
    cascade->categories_size = 0;
    cascade->bloomfilters_size = 0;
    cascade->categories_names = NULL;
    cascade->bloomfilters = NULL;
    cascade->last_category_name = NULL;

    FILE* file = fopen(file_path, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", file_path);
        return clean_return(1, cascade, free_cascade);
    }

    size_t read_size;
    
    // parse categories names
    uint32_t category_size;
    read_size = fread(&category_size, sizeof(uint32_t), 1, file);
    if (read_size != 1) return read_error("category size", file_path, cascade, file, NULL);

    cascade->categories_size = category_size;
    cascade->categories_names = malloc(sizeof(char*) * category_size);
    if (!cascade->categories_names) {
        fprintf(stderr, "Failed to allocate categories names\n");
        return clean_return(2, cascade, free_cascade, file, fclose);
    }

    for (uint32_t i = 0; i < category_size; i++) {
        uint8_t length;
        read_size = fread(&length, sizeof(uint8_t), 1, file);
        if (read_size != 1) return read_error("category name length", file_path, cascade, file, NULL);

        char* name = malloc(sizeof(char) * (length + 1));
        if (!name) {
            fprintf(stderr, "Failed to allocate category name\n");
            return clean_return(2, cascade, free_cascade, file, fclose);
        }
        read_size = fread(name, sizeof(char), length, file);
        if (read_size != length) return read_error("category name", file_path, cascade, file, NULL);
        name[length] = '\0';

        cascade->categories_names[i] = name;
    }

    // parse bloomfilters
    cascade->bloomfilters = malloc(sizeof(Bloomfilter*) * category_size);
    if (!cascade->bloomfilters) {
        fprintf(stderr, "Failed to allocate bloomfilters\n");
        return clean_return(2, cascade, free_cascade, file, fclose);
    }

    uint32_t unfinished_amount = 0;
    uint32_t i = 0;

    uint8_t hash_amount;
    read_size = fread(&hash_amount, sizeof(uint8_t), 1, file);
    if (read_size != 1) return read_error("first hash amount", file_path, cascade, file, NULL);

    while (1) {
        if (hash_amount == 0xFF) break; // done

        // realloc if another step is created
        if (i != 0) {
            cascade->bloomfilters = realloc(cascade->bloomfilters, sizeof(Bloomfilter*) * category_size * (i + 1));
            if (!cascade->bloomfilters) {
                fprintf(stderr, "Failed to reallocate bloomfilters\n");
                return clean_return(2, cascade, free_cascade, file, fclose);
            }
        }

        // parse the bloomfilters for this step
        for (uint32_t j = 0; j < category_size; j++) {
            Bloomfilter* bloomfilter = NULL;

            if (hash_amount == 0xFF) {
                unfinished_amount = category_size - j;
                break;
            }

            // non-empty bloomfilter
            if (hash_amount != 0) {
                bloomfilter = malloc(sizeof(Bloomfilter));
                if (!bloomfilter) {
                    fprintf(stderr, "Memory allocation of bloomfilter failed\n");
                    return clean_return(2, cascade, free_cascade, file, fclose);
                }

                // handle hash seeds
                bloomfilter->hash_amount = hash_amount;
                bloomfilter->hash_seeds = calloc(hash_amount, sizeof(uint8_t));
                if (!bloomfilter->hash_seeds) {
                    fprintf(stderr, "Memory allocation of bloomfilter hash seeds failed\n");
                    return clean_return(3, cascade, free_cascade, file, fclose, bloomfilter, free_bloomfilter);
                }

                for (uint8_t k = 0; k < hash_amount; k++) {
                    uint8_t hash_seed;
                    read_size = fread(&hash_seed, sizeof(uint8_t), 1, file);
                    if (read_size != 1) return read_error("hash seed", file_path, cascade, file, bloomfilter);
                    bloomfilter->hash_seeds[k] = hash_seed;
                }

                // handle bf bits
                uint32_t bf_size;
                read_size = fread(&bf_size, sizeof(uint32_t), 1, file);
                if (read_size != 1) return read_error("bf size", file_path, cascade, file, bloomfilter);
                
                bloomfilter->size = bf_size;
                bloomfilter->bf = calloc(bf_size, sizeof(uint8_t));
                if (!bloomfilter->bf) {
                    fprintf(stderr, "Memory allocation of bloomfilter bits failed\n");
                    return clean_return(3, cascade, free_cascade, file, fclose, bloomfilter, free_bloomfilter);
                }

                for (uint32_t k = 0; k < bf_size; k++) {
                    uint8_t bf_part;
                    read_size = fread(&bf_part, sizeof(uint8_t), 1, file);
                    if (read_size != 1) return read_error("bf", file_path, cascade, file, bloomfilter);
                    bloomfilter->bf[k] = bf_part;
                }
            }

            cascade->bloomfilters[category_size * i + j] = bloomfilter;

            // read hash amount for next iteration
            read_size = fread(&hash_amount, sizeof(uint8_t), 1, file);
            if (read_size != 1) return read_error("hash amount", file_path, cascade, file, NULL);
        }

        i++;
    }

    cascade->bloomfilters_size = i * cascade->categories_size - unfinished_amount;

    // parse last categories name
    uint8_t length;
    read_size = fread(&length, sizeof(uint8_t), 1, file);
    if (read_size != 1) return read_error("last category name length", file_path, cascade, file, NULL);

    if (length != 0) {
        char* name = malloc(sizeof(char) * (length + 1));
        if (!name) {
            fprintf(stderr, "Failed to allocate last category name\n");
            return clean_return(2, cascade, free_cascade, file, fclose);
        }
        read_size = fread(name, sizeof(char), length, file);
        if (read_size != length) return read_error("last category name", file_path, cascade, file, NULL);
        name[length] = '\0';

        cascade->last_category_name = name;
    }

    fclose(file);

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
        if (cascade->bloomfilters) {
            for (uint32_t i = 0; i < cascade->bloomfilters_size; i++)
                if (cascade->bloomfilters[i])
                    free_bloomfilter(cascade->bloomfilters[i]);
            free(cascade->bloomfilters);
        }
        if (cascade->last_category_name)
            free(cascade->last_category_name);
        free(cascade);
    }
}