#include "avium/core.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avium/internal.h"

Type AvmObjectType(object_t self) {
    if (self == NULL) {
        panic("Parameter `self` was `NULL`.");
    }

    return *(Type*)self;
}

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

never AvmVirtualFunctionTrap(const char8_t* function, Type type) {
    fprintf(stderr,
            "Attempted to call unimplemented virtual function: %s on type %s.",
            function, type->name);
    panic("Unimplemented virtual function trap triggered.");
}

bool AvmObjectEq(object_t lhs, object_t rhs) {
    function_t method = AvmObjectType(lhs)->vptr[FUNC_EQ];

    size_t size = AvmObjectSize(lhs);

    if (method == NULL) {
        return memcmp(lhs, rhs, size) == 0;
    }

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

    if (method == NULL) {
        AvmVirtualFunctionTrap(__func__, AvmObjectType(object));
    }

    return ((size_t(*)(object_t))method)(object);
}

size_t AvmGetCapacity(object_t object) {
    function_t method = AvmObjectType(object)->vptr[FUNC_GET_CAPACITY];

    if (method == NULL) {
        AvmVirtualFunctionTrap(__func__, AvmObjectType(object));
    }

    return ((size_t(*)(object_t))method)(object);
}

object_t AvmClone(object_t object) {
    function_t method = AvmObjectType(object)->vptr[FUNC_CLONE];

    if (method == NULL) {
        size_t size = AvmObjectSize(object);
        return memcpy(malloc(size), object, size);
    }

    return ((object_t(*)(object_t))method)(object);
}

AvmString AvmToString(object_t object) {
    function_t method = AvmObjectType(object)->vptr[FUNC_TO_STRING];

    if (method == NULL) {
        AvmVirtualFunctionTrap(__func__, AvmObjectType(object));
    }

    return ((object_t(*)(object_t))method)(object);
}

AVMAPI AvmResult AvmSuccess(object_t value);
AVMAPI AvmResult AvmFailure(AvmErrorKind kind);
AVMAPI bool AvmIsFailure(AvmResult self);
AVMAPI object_t AvmUnwrap(AvmResult self);
AVMAPI AvmOptional AvmSome(object_t value);
AVMAPI AvmOptional AvmNone();
AVMAPI bool AvmHasValue(AvmOptional optional);

void AvmMemCopy(uint8_t* source, size_t length, uint8_t* destination,
                size_t size) {
    if (source == NULL) {
        panic(SourceNullMsg);
    }

    if (destination == NULL) {
        panic(DestNullMsg);
    }

    size_t trueLength = length > size ? size : length;
    memcpy(destination, source, trueLength);
}
