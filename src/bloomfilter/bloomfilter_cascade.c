#include "bloomfilter.h"

void free_elements(Category* category) {
    if (category) {
        if (category->elements) {
            for (uint32_t i = 0; i < category->size; i++)
                free(category->elements[i]);
            free(category->elements);
        }
    }
}

uint8_t create_bloomfilter_cascade(CategoryList* list, char* file_path, uint8_t p) {
    FILE* file = write_start(list, file_path);
    if (!file) return 1;

    uint32_t empty_count = 0;

    while (empty_count < list->categories_size - 1) {
        for (uint32_t i = 0; i < list->categories_size; i++) {
            Category* category = list->categories[i];
            uint32_t old_size = category->size;

            // just write 8 x 0-bits if size is 0
            if (old_size == 0) {
                fwrite(&(uint8_t){0}, sizeof(uint8_t), 1, file);
                if (i == 0) empty_count++;
            } else {
                // create and write bloomfilter
                Bloomfilter* bloomfilter = create_bloomfilter(list, i, p);
                write_bloomfilter(bloomfilter, file);
                printf("Wrote bloomfilter for %s to file\n    - size: %d -- hash: %d\n", list->categories[i]->name, bloomfilter->size, bloomfilter->hash_amount);
                
                // update list
                // create new elements pointer to overwrite the new one
                uint32_t new_size = 0;
                uint32_t new_leftover_size = 32;
                char** new_elements = malloc(sizeof(char*) * new_leftover_size);
                if (!new_elements) {
                    fprintf(stderr, "Memory allocation of new elements failed");
                    clean_return(2, file, fclose, bloomfilter, free_bloomfilter);
                    return 1;
                }

                // tests all elements from category and put the false positives in the new one
                for (uint32_t j = 0; j < category->size; j++) {
                    char* element = category->elements[j];
                    uint8_t element_length = strlen(element);
                    uint8_t count = 0;
                    for (int8_t h = 0; h < bloomfilter->hash_amount; h++) {
                        uint32_t hash = murmurhash(element, element_length, bloomfilter->hash_seeds[h]) % (bloomfilter->size * 8);
                        if (bloomfilter->bf[hash / 8] & (1ULL << (hash % 8))) count++;
                    }
                    if (count == bloomfilter->hash_amount) {
                        // we got a false positive, add to the new elements
                        new_elements[new_size] = malloc(sizeof(char) * (element_length + 1));
                        if (!new_elements[new_size]) {
                            fprintf(stderr, "Memory allocation of new element failed");
                            clean_return(2, file, fclose, bloomfilter, free_bloomfilter);
                            return 1;
                        }
                        memcpy(new_elements[new_size], element, element_length);
                        new_elements[new_size][element_length] = '\0';
                        new_size++;
                        new_leftover_size--;
                
                        if (new_leftover_size == 0) {
                            new_elements = realloc(new_elements, sizeof(char*) * new_size * 2);
                            if (!new_elements) {
                                fprintf(stderr, "Memory reallocation of new elements failed");
                                clean_return(2, file, fclose, bloomfilter, free_bloomfilter);
                                return 1;
                            }
                            new_leftover_size = new_size;
                        }
                    }
                }

                free_bloomfilter(bloomfilter);

                // free old elements and set new values
                free_elements(category);
                category->size = new_size;
                category->leftover_size = new_leftover_size;
                category->elements = new_elements;
                list->elements_size -= old_size - new_size;

                if (new_size == 0) empty_count++;
            }
        }
    }

    char* non_empty_name = "";
    for (uint32_t i = 0; i < list->categories_size; i++) {
        if (list->categories[i]->size > 0) {
            non_empty_name = list->categories[i]->name;
            break;
        }
    }
    write_end(non_empty_name, file);

    printf("Succesfully wrote bloomfilter cascade to: %s\n", file_path);
    return 0;
}