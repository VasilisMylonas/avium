#include "avium/alloc.h"

#include <stdlib.h>
#include <string.h>

void* AvmAlloc(size_t size) { return malloc(size); }
void* AvmRealloc(void* memory, size_t size) { return realloc(memory, size); }
void AvmDealloc(void* memory) { free(memory); }

object AvmObjectAlloc(size_t size, object data) {
    object memory = malloc(size);
    if (data != NULL) {
        memcpy(memory, data, size);
    }
    return memory;
}

void AvmObjectDealloc(object memory) {
    if (memory != NULL) {
        AvmObjectDestroy(memory);
        free(memory);
    }
}
