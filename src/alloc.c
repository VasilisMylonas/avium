#include "avium/alloc.h"

#include <stdlib.h>
#include <string.h>

object AvmAlloc(size_t size, object data) {
    object memory = malloc(size);
    if (data != NULL) {
        memcpy(memory, data, size);
    }
    return memory;
}
