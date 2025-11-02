#include "clean_return.h"

void* clean_return(uint8_t count, ...) {
    fprintf(stderr, "Freeing allocated memory...\n");

    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        void* ptr = va_arg(args, void*);
        cleanup_func func = va_arg(args, cleanup_func);
        if (ptr && func)
            func(ptr);
    }

    va_end(args);
    return NULL;
}