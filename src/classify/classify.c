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

        printf("%s\n", cascade->categories_names[0]);
    }
}