#include "avium/alloc.h"

#include <stdlib.h>
#include <string.h>

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
