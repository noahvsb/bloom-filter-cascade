#include "bloomfilter.h"

uint64_t* create_bloomfilter(CategoryList* list, uint64_t except) {
    uint32_t n = 1; // amount of bits / 64 (TODO)
    uint16_t k = 3; // amount of hashfunctions (TODO)
    uint64_t* bloomfilter = calloc(n, sizeof(uint64_t));
    if (!bloomfilter) {
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
                bloomfilter[0] |= (1ULL << hash); // TODO: if n > 1, fix
            }
        }
    }
    return bloomfilter; // TODO: return bloomfilter
}
