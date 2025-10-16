#include "bloomfilter.h"

Bloomfilter* create_bloomfilter(CategoryList* list, uint64_t except) {
    uint32_t n = 5; // amount of bits / 64 (TODO)
    uint16_t k = 5; // amount of hashfunctions (TODO)

    Bloomfilter* bloomfilter = malloc(sizeof(Bloomfilter));
    if (!bloomfilter) {
        fprintf(stderr, "Memory allocation of bloomfilter failed\n");
        return NULL;
    }
    bloomfilter->size = n;
    bloomfilter->bf = calloc(n, sizeof(uint64_t));
    if (!bloomfilter->bf) {
        fprintf(stderr, "Memory allocation of bloomfilter failed\n");
        return NULL;
    }

    for (int64_t i = 0; i < list->categories_size; i++) {
        if (i == except) continue;
        Category* category = list->categories[i];
        for (int64_t j = 0; j < category->size; j++) {
            char* element = category->elements[j];
            uint8_t element_length = strlen(element);
            for (int16_t l = 0; l < k; l++) {
                uint32_t hash = murmurhash(element, element_length, l) % (n * 64);
                printf("hash for %s with seed %d: %d\n", element, l, hash);
                bloomfilter->bf[hash / 64] |= (1ULL << (hash % 64));
            }
        }
    }

    return bloomfilter;
}
