#include "avium/core.h"

#include <stdio.h>
#include <stdlib.h>

#include "avium/internal.h"

Type AvmObjectType(object_t self) { return *(Type*)self; }
const char8_t* AvmObjectName(object_t self) {
    return AvmObjectType(self)->name;
}
size_t AvmObjectSize(object_t self) { return AvmObjectType(self)->size; }

never AvmPanic(const char8_t* message, const char8_t* function,
               const char8_t* file, uint32_t line) {
    fprintf(stderr, "Panic in file %s:%u in function %s()\n%s\n", file, line,
            function, message);
    abort();
}

bool AvmObjectEq(object_t lhs, object_t rhs) {
    function_t method = AvmObjectType(lhs)->vptr[FUNC_EQ];
    return ((bool (*)(object_t, object_t))method)(lhs, rhs);
}

void AvmDestroy(object_t object) {
    function_t method = AvmObjectType(object)->vptr[FUNC_DTOR];

    if (method == NULL) {
        free(object);
        return;
    }

    ((void (*)(object_t))method)(object);
}

size_t AvmGetLength(object_t object) {
    function_t method = AvmObjectType(object)->vptr[FUNC_GET_LENGTH];
    return ((size_t(*)(object_t))method)(object);
}

size_t AvmGetCapacity(object_t object) {
    function_t method = AvmObjectType(object)->vptr[FUNC_GET_CAPACITY];
    return ((size_t(*)(object_t))method)(object);
}

object_t AvmClone(object_t object) {
    function_t method = AvmObjectType(object)->vptr[FUNC_CLONE];
    return ((object_t(*)(object_t))method)(object);
}

AvmString AvmToString(object_t object) {
    function_t method = AvmObjectType(object)->vptr[FUNC_TO_STRING];
    return ((object_t(*)(object_t))method)(object);
}
