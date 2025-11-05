#include "bloomfilter.h"

Bloomfilter* create_bloomfilter(CategoryList* list, int32_t except, uint8_t k) {
    uint32_t except_size = except < 0 || except >= list->categories_size ? 0 : list->categories[except]->size;
    uint32_t m = list->elements_size - except_size;

    // adaptive calculation of k
    // TODO

    // calculation of n
    uint32_t n8 = m * k / log(2); // amount of bits (n = m * k / ln 2 rounded down)
    uint32_t n = (n8 + 7) / 8; // amount of bits / 8 rounded up

    Bloomfilter* bloomfilter = malloc(sizeof(Bloomfilter));
    if (!bloomfilter) {
        fprintf(stderr, "Memory allocation of bloomfilter failed\n");
        return NULL;
    }
    bloomfilter->size = n;
    bloomfilter->hash_amount = k;
    bloomfilter->bf = calloc(n, sizeof(uint8_t));
    bloomfilter->hash_seeds = calloc(k, sizeof(uint8_t));
    if (!bloomfilter->bf || !bloomfilter->hash_seeds) {
        fprintf(stderr, "Memory allocation of bloomfilter bits or hash seeds failed\n");
        return clean_return(1, bloomfilter, free_bloomfilter);
    }

    // generate hash seeds
    for (uint8_t i = 0; i < k; i++) {
        bloomfilter->hash_seeds[i] = (uint8_t) (rand() % 256);
    }

    for (int32_t i = 0; i < list->categories_size; i++) {
        if (i == except) continue;
        Category* category = list->categories[i];
        for (int32_t j = 0; j < category->size; j++) {
            char* element = category->elements[j];
            uint8_t element_length = strlen(element);
            for (int8_t h = 0; h < k; h++) {
                uint32_t hash = murmurhash(element, element_length, bloomfilter->hash_seeds[h]) % (n * 8);
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
        if (bloomfilter->hash_seeds)
            free(bloomfilter->hash_seeds);
        free(bloomfilter);
    }
}
