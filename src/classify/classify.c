#include "classify.h"

// split up so this can be used in tests
char* classify(Cascade* cascade, char* element_name) {
    char* category_name = NULL;

    bool done = false;
    for (uint32_t i = 0; i < cascade->bloomfilters_size && !done; i++) {
        Bloomfilter* bloomfilter = cascade->bloomfilters[i];

        if (bloomfilter == NULL) continue;

        uint8_t count = 0;
        for (int8_t h = 0; h < bloomfilter->hash_amount; h++) {
            uint32_t hash = murmurhash(element_name, strlen(element_name), bloomfilter->hash_seeds[h]) % (bloomfilter->size * 8);
            if (bloomfilter->bf[hash / 8] & (1ULL << (hash % 8))) count++;
        }

        if (count != bloomfilter->hash_amount) {
            category_name = cascade->categories_names[i % cascade->categories_size];
            done = true;
        }
    }

    // if one couldn't be classified, it has to be in the last non-empty category
    if (category_name == NULL) category_name = cascade->last_category_name;

    return category_name;
}

void run_classify(Cascade* cascade) {
    while (1) {
        char element_name[256];

        if (fgets(element_name, sizeof(element_name), stdin) == NULL) {
            fprintf(stderr, "Failed to read input\n");
            return;
        }

        element_name[strcspn(element_name, "\n")] = '\0';

        if (strcmp(element_name, ":q") == 0) {
            printf("Done!\n");
            return;
        }

        char* category_name = classify(cascade, element_name);

        if (category_name == NULL)
            fprintf(stderr, "Error: couldn't classify %s\n", element_name);
        else
            printf("%s\n", category_name);
    }
}