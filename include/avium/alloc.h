#ifndef AVIUM_ALLOC_H
#define AVIUM_ALLOC_H

#include "avium/types.h"

AVMAPI object AvmAlloc(size_t size, object data);

#define heapalloc(T, ...) AvmAlloc(sizeof(T), (T[1]){__VA_ARGS__})

#endif  // AVIUM_ALLOC_H
