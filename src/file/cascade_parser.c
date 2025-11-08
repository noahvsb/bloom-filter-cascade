#include "cascade_parser.h"

void free_fast_cascade(FastCascade* fast_cascade) {
    if (fast_cascade) {
        if (fast_cascade->categories_names) {
            for (uint32_t i = 0; i < fast_cascade->categories_size; i++)
                if (fast_cascade->categories_names[i])
                    free(fast_cascade->categories_names[i]);
            free(fast_cascade->categories_names);
        }
        if (fast_cascade->bloomfilters) {
            for (uint32_t i = 0; i < fast_cascade->bloomfilters_size; i++)
                if (fast_cascade->bloomfilters[i])
                    free_bloomfilter(fast_cascade->bloomfilters[i]);
            free(fast_cascade->bloomfilters);
        }
        if (fast_cascade->last_category_name)
            free(fast_cascade->last_category_name);
        free(fast_cascade);
    }
}

static void* read_error_fast(char* msg, char* file_path, FastCascade* fast_cascade, Bloomfilter* bloomfilter) {
    fprintf(stderr, "Failed to read %s from file: %s\n", msg, file_path);
    return clean_return(2, fast_cascade, free_fast_cascade, bloomfilter, free_bloomfilter);
}

FastCascade* parse_fast_cascade(FILE* file, char* file_path) {
    FastCascade* fast_cascade = malloc(sizeof(FastCascade));
    if (!fast_cascade) {
        fprintf(stderr, "Failed to allocate fast cascadfe\n");
        return NULL;
    }

    fast_cascade->categories_size = 0;
    fast_cascade->bloomfilters_size = 0;
    fast_cascade->categories_names = NULL;
    fast_cascade->bloomfilters = NULL;
    fast_cascade->last_category_name = NULL;

    uint8_t read_size;
    
    // parse categories names
    uint32_t category_size;
    read_size = fread(&category_size, sizeof(uint32_t), 1, file);
    if (read_size != 1) return read_error_fast("category size", file_path, fast_cascade, NULL);

    fast_cascade->categories_size = category_size;
    fast_cascade->categories_names = malloc(sizeof(char*) * category_size);
    if (!fast_cascade->categories_names) {
        fprintf(stderr, "Failed to allocate categories names\n");
        return clean_return(1, fast_cascade, free_fast_cascade);
    }

    for (uint32_t i = 0; i < category_size; i++) {
        uint8_t length;
        read_size = fread(&length, sizeof(uint8_t), 1, file);
        if (read_size != 1) return read_error_fast("category name length", file_path, fast_cascade, NULL);

        char* name = malloc(sizeof(char) * (length + 1));
        if (!name) {
            fprintf(stderr, "Failed to allocate category name\n");
            return clean_return(1, fast_cascade, free_fast_cascade);
        }
        read_size = fread(name, sizeof(char), length, file);
        if (read_size != length) return read_error_fast("category name", file_path, fast_cascade, NULL);
        name[length] = '\0';

        fast_cascade->categories_names[i] = name;
    }

    // parse bloomfilters
    fast_cascade->bloomfilters = malloc(sizeof(Bloomfilter*) * category_size);
    if (!fast_cascade->bloomfilters) {
        fprintf(stderr, "Failed to allocate bloomfilters\n");
        return clean_return(1, fast_cascade, free_fast_cascade);
    }

    uint32_t unfinished_amount = 0;
    uint32_t i = 0;

    uint8_t hash_amount;
    read_size = fread(&hash_amount, sizeof(uint8_t), 1, file);
    if (read_size != 1) return read_error_fast("first hash amount", file_path, fast_cascade, NULL);

    while (1) {
        if (hash_amount == 0xFF) break; // done

        // realloc if another step is created
        if (i != 0) {
            fast_cascade->bloomfilters = realloc(fast_cascade->bloomfilters, sizeof(Bloomfilter*) * category_size * (i + 1));
            if (!fast_cascade->bloomfilters) {
                fprintf(stderr, "Failed to reallocate bloomfilters\n");
                return clean_return(1, fast_cascade, free_fast_cascade);
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
                    return clean_return(1, fast_cascade, free_fast_cascade);
                }

                // handle hash seeds
                bloomfilter->hash_amount = hash_amount;
                bloomfilter->hash_seeds = calloc(hash_amount, sizeof(uint8_t));
                if (!bloomfilter->hash_seeds) {
                    fprintf(stderr, "Memory allocation of bloomfilter hash seeds failed\n");
                    return clean_return(2, fast_cascade, free_fast_cascade, bloomfilter, free_bloomfilter);
                }

                for (uint8_t k = 0; k < hash_amount; k++) {
                    uint8_t hash_seed;
                    read_size = fread(&hash_seed, sizeof(uint8_t), 1, file);
                    if (read_size != 1) return read_error_fast("hash seed", file_path, fast_cascade, bloomfilter);
                    bloomfilter->hash_seeds[k] = hash_seed;
                }

                // handle bf bits
                uint32_t bf_size;
                read_size = fread(&bf_size, sizeof(uint32_t), 1, file);
                if (read_size != 1) return read_error_fast("bf size", file_path, fast_cascade, bloomfilter);
                
                bloomfilter->size = bf_size;
                bloomfilter->bf = calloc(bf_size, sizeof(uint8_t));
                if (!bloomfilter->bf) {
                    fprintf(stderr, "Memory allocation of bloomfilter bits failed\n");
                    return clean_return(2, fast_cascade, free_fast_cascade, bloomfilter, free_bloomfilter);
                }

                for (uint32_t k = 0; k < bf_size; k++) {
                    uint8_t bf_part;
                    read_size = fread(&bf_part, sizeof(uint8_t), 1, file);
                    if (read_size != 1) return read_error_fast("bf", file_path, fast_cascade, bloomfilter);
                    bloomfilter->bf[k] = bf_part;
                }
            }

            fast_cascade->bloomfilters[category_size * i + j] = bloomfilter;

            // read hash amount for next iteration
            read_size = fread(&hash_amount, sizeof(uint8_t), 1, file);
            if (read_size != 1) return read_error_fast("hash amount", file_path, fast_cascade, NULL);
        }

        i++;
    }

    fast_cascade->bloomfilters_size = i * fast_cascade->categories_size - unfinished_amount;

    // parse last categories name
    uint8_t length;
    read_size = fread(&length, sizeof(uint8_t), 1, file);
    if (read_size != 1) return read_error_fast("last category name length", file_path, fast_cascade, NULL);

    if (length != 0) {
        char* name = malloc(sizeof(char) * (length + 1));
        if (!name) {
            fprintf(stderr, "Failed to allocate last category name\n");
            return clean_return(1, fast_cascade, free_fast_cascade);
        }
        read_size = fread(name, sizeof(char), length, file);
        if (read_size != length) return read_error_fast("last category name", file_path, fast_cascade, NULL);
        name[length] = '\0';

        fast_cascade->last_category_name = name;
    }

    return fast_cascade;
}

Cascade* parse_cascade(char* file_path) {
    FILE* file = fopen(file_path, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", file_path);
        return NULL;
    }

    Cascade* cascade = malloc(sizeof(Cascade));
    if (!cascade) {
        fprintf(stderr, "Failed to allocate cascade\n");
        return clean_return(1, file, fclose);
    }
    cascade->algorithm = 0;
    cascade->fast = NULL;
    cascade->less_storage = NULL;

    size_t read_size;

    // algorithm
    bool algorithm;
    read_size = fread(&algorithm, sizeof(bool), 1, file);
    if (read_size != 1) {
        fprintf(stderr, "Failed to read algorithm from file: %s\n", file_path);
        return clean_return(2, cascade, free_cascade, file, fclose);
    }

    if (algorithm) {
        cascade->algorithm = algorithm;
        cascade->fast = parse_fast_cascade(file, file_path); // TODO: implement parse_less_storage_cascade()
        if (!cascade->fast) {
            return clean_return(2, cascade, free_cascade, file, fclose);
        }
    } else {
        cascade->algorithm = algorithm;
        cascade->fast = parse_fast_cascade(file, file_path);
        if (!cascade->fast) {
            return clean_return(2, cascade, free_cascade, file, fclose);
        }
    }

    fclose(file);

    return cascade;
}

void free_cascade(Cascade* cascade) {
    if (cascade) {
        if (cascade->algorithm)
            free_fast_cascade(cascade->fast); // TODO: implement free_less_storage_cascade()
        else
            free_fast_cascade(cascade->fast);
        free(cascade);
    }
}