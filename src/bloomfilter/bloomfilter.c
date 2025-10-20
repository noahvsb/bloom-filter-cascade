#include "bloomfilter.h"

Bloomfilter* create_bloomfilter(CategoryList* list, int32_t except, uint8_t p) {
    uint32_t n8 = list->elements_size * p; // amount of bits (n = m * p)
    uint32_t n = (n8 + 7) / 8; // amount of bits / 8 rounded up
    uint8_t k = p * log(2); // amount of hashfunctions (k = n/m * ln(2) rounded down = p * ln(2) rounded down)

    Bloomfilter* bloomfilter = malloc(sizeof(Bloomfilter));
    if (!bloomfilter) {
        fprintf(stderr, "Memory allocation of bloomfilter failed\n");
        return clean_return(1, bloomfilter, free_bloomfilter);
    }
    bloomfilter->size = n;
    bloomfilter->bf = calloc(n, sizeof(uint8_t));
    if (!bloomfilter->bf) {
        fprintf(stderr, "Memory allocation of bloomfilter failed\n");
        return clean_return(1, bloomfilter, free_bloomfilter);
    }

    for (int32_t i = 0; i < list->categories_size; i++) {
        if (i == except) continue;
        Category* category = list->categories[i];
        for (int32_t j = 0; j < category->size; j++) {
            char* element = category->elements[j];
            uint8_t element_length = strlen(element);
            for (int8_t l = 0; l < k; l++) {
                uint32_t hash = murmurhash(element, element_length, l) % (n * 8);
                bloomfilter->bf[hash / 8] |= (1ULL << (hash % 8));
            }
        }
    }

    return bloomfilter;
}

void free_bloomfilter(Bloomfilter* bloomfilter) {
    if (bloomfilter) {
        if (bloomfilter->bf)
            free(bloomfilter->bf);
        free(bloomfilter);
    }
}
