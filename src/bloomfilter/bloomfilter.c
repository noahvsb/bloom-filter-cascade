#include "bloomfilter.h"

Bloomfilter* create_bloomfilter(CategoryList* list, int64_t except, uint8_t p) {
    uint64_t n8 = list->elements_size * p; // amount of bits (n = m * p)
    uint64_t n = (n8 + 7) / 8; // amount of bits / 8 rounded up
    uint8_t k = p * log(2); // amount of hashfunctions (k = n/m * ln(2) rounded down = p * ln(2) rounded down)

    Bloomfilter* bloomfilter = malloc(sizeof(Bloomfilter));
    if (!bloomfilter) {
        fprintf(stderr, "Memory allocation of bloomfilter failed\n");
        return NULL;
    }
    bloomfilter->size = n;
    bloomfilter->bf = calloc(n, sizeof(uint8_t));
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
            for (int8_t l = 0; l < k; l++) {
                uint64_t hash = murmurhash(element, element_length, l) % (n * 8);
                printf("hash for %s with seed %d: %ld\n", element, l, hash);
                bloomfilter->bf[hash / 8] |= (1ULL << (hash % 8));
            }
        }
    }

    return bloomfilter;
}
