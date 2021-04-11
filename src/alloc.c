#include "avium/alloc.h"

// #include <stdlib.h>
#include <string.h>

#ifdef AVM_USE_GC
#    include <gc/gc.h>
#    define AVM_ALLOC   GC_malloc
#    define AVM_REALLOC GC_realloc
#    define AVM_DEALLOC GC_free
#else
#    include <stdlib.h>
#    define AVM_ALLOC   malloc
#    define AVM_REALLOC realloc
#    define AVM_DEALLOC free
#endif

void* AvmAlloc(size_t size) { return AVM_ALLOC(size); }
void* AvmRealloc(void* memory, size_t size) {
    return AVM_REALLOC(memory, size);
}
void AvmDealloc(void* memory) { AVM_DEALLOC(memory); }

object AvmObjectAlloc(size_t size, object data) {
    object memory = AvmAlloc(size);
    if (data != NULL) {
        memcpy(memory, data, size);
    }
    return memory;
}
