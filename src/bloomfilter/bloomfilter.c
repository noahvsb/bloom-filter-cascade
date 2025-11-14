#include "bloomfilter.h"

void set_bits(Bloomfilter* bloomfilter, Category* category) {
    for (int32_t j = 0; j < category->size; j++) {
        char* element = category->elements[j];
        uint8_t element_length = strlen(element);
        for (int8_t h = 0; h < bloomfilter->hash_amount; h++) {
            uint32_t hash = murmurhash(element, element_length, bloomfilter->hash_seeds[h]) % (bloomfilter->size * 8);
            bloomfilter->bf[hash / 8] |= (1ULL << (hash % 8));
        }
    }
}

Bloomfilter* create_bloomfilter(CategoryList* list, int32_t except, int32_t only, uint8_t k) {
    bool use_except = except >= 0 && except < list->categories_size;
    bool use_only   = only   >= 0 && only   < list->categories_size;
    if (use_except && use_only) use_only = false;

    // calculate amount of elements
    uint32_t except_size = use_except && list->categories[except] ? list->categories[except]->size : 0;
    uint32_t m = use_only ? list->categories[only]->size : list->elements_size - except_size;

    // calculate amount of bits
    uint32_t n8 = m * k / log(2); // amount of bits (n = m * k / ln 2 rounded down)
    uint32_t n  = (n8 + 7) / 8; // amount of bits / 8 rounded up

    // alloc and initialize bloomfilter
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
    for (uint8_t i = 0; i < k; i++)
        bloomfilter->hash_seeds[i] = (uint8_t) (rand() % 256);

    // set bits
    if (use_only)
        set_bits(bloomfilter, list->categories[only]);
    else {
        for (int32_t i = 0; i < list->categories_size; i++) {
            if (i == except) continue;
            set_bits(bloomfilter, list->categories[i]);
        }
    }

    return bloomfilter;
}

bool test_bloomfilter(Bloomfilter* bloomfilter, char* element_name) {
    uint8_t count = 0;
    
    for (int8_t h = 0; h < bloomfilter->hash_amount; h++) {
        uint32_t hash = murmurhash(element_name, strlen(element_name), bloomfilter->hash_seeds[h]) % (bloomfilter->size * 8);
        if (bloomfilter->bf[hash / 8] & (1ULL << (hash % 8))) count++;
    }

    return count == bloomfilter->hash_amount;
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
