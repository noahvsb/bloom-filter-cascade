#ifndef CLEAN_RETURN_H
#define CLEAN_RETURN_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

typedef void (*cleanup_func)(void *); // generic custom cleanup function type

/**
 * cleans up pointers using custom free functions
 * 
 * @param count: amount of pointers
 * 
 * redo count times in the same order:
 * @param ptr: pointer which has to be freed
 * @param func: custom free function that frees said pointer
 */
void *clean_return(uint8_t count, ...);

#endif