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

uint8_t update_category(Category* category, Bloomfilter* bloomfilter, Category* output) {
    // create new elements pointer to overwrite the new one
    uint32_t new_size = 0;
    uint32_t new_leftover_size = 32;
    char** new_elements = malloc(sizeof(char*) * new_leftover_size);
    if (!new_elements) {
        fprintf(stderr, "Memory allocation of new elements failed");
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
                return 1;
            }
            memcpy(new_elements[new_size], element, element_length + 1);
            new_size++;
            new_leftover_size--;
    
            if (new_leftover_size == 0) {
                new_elements = realloc(new_elements, sizeof(char*) * new_size * 2);
                if (!new_elements) {
                    fprintf(stderr, "Memory reallocation of new elements failed");
                    return 1;
                }
                new_leftover_size = new_size;
            }
        }
    }

    if (output == NULL) output = category;

    // free old elements and set new values
    free_elements(output);
    output->size = new_size;
    output->leftover_size = new_leftover_size;
    output->elements = new_elements;

    return 0;
}

uint8_t fast_algorithm(FILE* file, CategoryList* list) {
    bool first_step = true;
    uint32_t empty_count = 0;

    while (empty_count < list->categories_size - 1) {
        for (uint32_t i = 0; i < list->categories_size && empty_count < list->categories_size - 1; i++) {
            Category* category = list->categories[i];
            uint32_t old_size = category->size;

            // just write 8 x 0-bits if size is 0
            if (old_size == 0) {
                fwrite(&(uint8_t){0}, sizeof(uint8_t), 1, file);
                if (first_step) empty_count++;
            } else {
                // create and write bloomfilter
                Bloomfilter* bloomfilter = create_bloomfilter(list, i, -1, 1);
                if (!bloomfilter) return !clean_return(1, file, fclose);
                write_bloomfilter(bloomfilter, file);
                printf("Wrote bloomfilter for %s to file\n    - size: %d -- hash: %d\n", list->categories[i]->name, bloomfilter->size, bloomfilter->hash_amount);
                
                // update category
                if (update_category(category, bloomfilter, NULL)) return !clean_return(2, file, fclose, bloomfilter, free_bloomfilter);
                list->elements_size -= old_size - category->size;

                if (category->size == 0) empty_count++;

                free_bloomfilter(bloomfilter);
            }
        }

        printf("\n");

        first_step = false;
    }

    char* non_empty_name = "";
    for (uint32_t i = 0; i < list->categories_size; i++) {
        if (list->categories[i]->size > 0) {
            non_empty_name = list->categories[i]->name;
            break;
        }
    }
    write_end(non_empty_name, file);

    return 0;
}

uint8_t less_storage_algorithm(FILE* file, CategoryList* list) {
    bool first_step = true;
    uint32_t empty_count = 0;

    while (empty_count < list->categories_size - 1 && list->categories_size > 0) {
        // loop for 1 cascade step
        for (uint32_t i = 0; i < list->categories_size && empty_count < list->categories_size - 1; i++) {
            Category* category = list->categories[i];
            uint32_t old_size = category->size;

            // just write 16 x 0-bits if size is 0, because in this algorithm, that means 2 empty bloomfilters
            if (old_size == 0) {
                fwrite(&(uint16_t){0}, sizeof(uint16_t), 1, file);
                if (first_step) empty_count++;
            } else {
                // create and write first bloomfilter
                Bloomfilter* bloomfilter1 = create_bloomfilter(list, -1, i, 3);
                if (!bloomfilter1) return !clean_return(1, file, fclose);
                write_bloomfilter(bloomfilter1, file);
                printf("Wrote bloomfilter 1 for %s to file\n    - size: %d -- hash: %d\n", list->categories[i]->name, bloomfilter1->size, bloomfilter1->hash_amount);

                // create temporary list to create the next bloomfilter
                CategoryList* temp = malloc(sizeof(CategoryList));
                if (!temp) {
                    fprintf(stderr, "Memory allocation of temporary list failed\n");
                    return !clean_return(2, file, fclose, bloomfilter1, free_bloomfilter);
                }
                temp->elements_size = 0;
                temp->categories_size = list->categories_size;
                temp->leftover_size = 0;
                temp->categories = malloc(sizeof(Category*) * temp->categories_size);
                if (!temp->categories) {
                    fprintf(stderr, "Memory allocation of temporary list categories failed\n");
                    return !clean_return(3, file, fclose, bloomfilter1, free_bloomfilter, temp, free_categories);
                }

                // update categories and store in temp
                for (uint32_t j = 0; j < list->categories_size; j++) {
                    if (j == i) {
                        temp->categories[j] = NULL;
                        continue;
                    }

                    temp->categories[j] = malloc(sizeof(Category));
                    if (!temp->categories[j]) {
                        fprintf(stderr, "Memory allocation of temporary category failed\n");
                        return !clean_return(3, file, fclose, bloomfilter1, free_bloomfilter, temp, free_categories);
                    }
                    temp->categories[j]->size = 0;
                    temp->categories[j]->leftover_size = 0;
                    temp->categories[j]->elements = NULL;
                    temp->categories[j]->name = NULL;

                    if (update_category(list->categories[j], bloomfilter1, temp->categories[j]))
                        return !clean_return(2, file, fclose, bloomfilter1, free_bloomfilter, temp, free_categories);

                    temp->elements_size += temp->categories[j]->size;
                }

                free_bloomfilter(bloomfilter1);

                // create and write second bloomfilter
                if (temp->elements_size == 0)
                    fwrite(&(uint8_t){0}, sizeof(uint8_t), 1, file); // write 8 x 0 bits if temp is empty
                else {
                    Bloomfilter* bloomfilter2 = create_bloomfilter(temp, i, -1, 4);
                    if (!bloomfilter2) {
                        return !clean_return(2, file, fclose, temp, free_categories);
                    }
                    write_bloomfilter(bloomfilter2, file);
                    printf("Wrote bloomfilter 2 for %s to file\n    - size: %d -- hash: %d\n", list->categories[i]->name, bloomfilter2->size, bloomfilter2->hash_amount);

                    // update category
                    if (update_category(category, bloomfilter2, NULL)) return !clean_return(3, file, fclose, temp, free_categories, bloomfilter2, free_bloomfilter);
                    list->elements_size -= old_size - category->size;

                    if (category->size == 0) empty_count++;

                    free_bloomfilter(bloomfilter2);
                }

                free_categories(temp);
            }
        }

        printf("\n");

        first_step = false;
    }

    char* non_empty_name = "";
    for (uint32_t i = 0; i < list->categories_size; i++) {
        if (list->categories[i]->size > 0) {
            non_empty_name = list->categories[i]->name;
            break;
        }
    }
    write_end(non_empty_name, file);

    return 0;
}

uint8_t create_bloomfilter_cascade(char* file_path, CategoryList* list, bool algorithm) {
    FILE* file = write_start(file_path, list, algorithm);
    if (!file) return 1;

    if (algorithm) {
        if (less_storage_algorithm(file, list)) return 1;
    } else {
        if (fast_algorithm(file, list)) return 1;
    }

    printf("Succesfully wrote bloomfilter cascade to: %s\n", file_path);
    return 0;
}