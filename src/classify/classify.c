#include "classify.h"

void classify(Cascade* cascade) {
    while (1) {
        char name[256];

        uint8_t scan_size = scanf("%s", name);
        if (scan_size != 1) {
            fprintf(stderr, "Failed to scan element name\n");
            return;
        }

        if (strcmp(name, ":q") == 0) {
            printf("Done!\n");
            return;
        }

        char* category_name = NULL;
        bool done = false;
        for (uint32_t i = 0; i < cascade->steps && !done; i++) {
            for (uint32_t j = 0; j < cascade->categories_size && !done; j++) {
                Bloomfilter* bloomfilter = cascade->bloomfilters[i * cascade->categories_size + j];

                uint8_t count = 0;
                for (int8_t h = 0; h < bloomfilter->hash_amount; h++) {
                    uint32_t hash = murmurhash(name, strlen(name), bloomfilter->hash_seeds[h]) % (bloomfilter->size * 8);
                    if (bloomfilter->bf[hash / 8] & (1ULL << (hash % 8))) count++;
                }

                if (count != bloomfilter->hash_amount) {
                    category_name = cascade->categories_names[j];
                    done = true;
                }
            }
        }

        // if one couldn't be classified, it has to be in the last non-empty category
        if (category_name == NULL) category_name = cascade->last_category_name;

        // this should only happen if the element isn't in the whole set
        if (category_name == NULL)
            fprintf(stderr, "Error: couldn't classify %s\n", name);
        else
            printf("%s\n", category_name);
    }
}