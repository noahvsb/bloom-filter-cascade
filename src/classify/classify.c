#include "classify.h"

char* classify_fast(FastCascade* fast_cascade, char* element_name) {
    char* category_name = NULL;

    bool done = false;
    for (uint32_t i = 0; i < fast_cascade->bloomfilters_size && !done; i++) {
        Bloomfilter* bloomfilter = fast_cascade->bloomfilters[i];

        if (bloomfilter == NULL) continue; // empty

        if (!test_bloomfilter(bloomfilter, element_name)) {
            category_name = fast_cascade->categories_names[i % fast_cascade->categories_size];
            done = true;
        }
    }

    // if one couldn't be classified, it has to be in the last non-empty category
    if (category_name == NULL) category_name = fast_cascade->last_category_name;

    return category_name;
}

char* classify_less_storage(FastCascade* cascade, char* element_name) {
    char* category_name = NULL;

    bool done = false;
    for (uint32_t i = 0; i < cascade->bloomfilters_size && !done; i+=2) {
        Bloomfilter* bloomfilter1 = cascade->bloomfilters[i];

        if (bloomfilter1 == NULL) continue; // empty

        if (!test_bloomfilter(bloomfilter1, element_name)) continue;

        Bloomfilter* bloomfilter2 = cascade->bloomfilters[i + 1];

        if (bloomfilter2 == NULL) continue; // empty

        if (!test_bloomfilter(bloomfilter2, element_name)) {
            category_name = cascade->categories_names[(i / 2) % cascade->categories_size];
            done = true;
        }
    }

    // if one couldn't be classified, it has to be in the last non-empty category
    if (category_name == NULL) category_name = cascade->last_category_name;

    return category_name;
}

char* classify(Cascade* cascade, char* element_name) {
    if (cascade->algorithm) return classify_less_storage(cascade->fast, element_name);
    else return classify_fast(cascade->fast, element_name);
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