#include "avium/object.h"
#include "avium/internal.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

AvmType AvmObjectType(object self) {
    if (self == NULL) {
        panic("Parameter `self` was `NULL`.");
    }

    return *(AvmType*)self;
}

str AvmObjectName(object self) { return AvmObjectType(self)->name; }

size_t AvmObjectSize(object self) { return AvmObjectType(self)->size; }

never AvmVirtualFunctionTrap(str function, AvmType type) {
    fprintf(stderr,
            "Attempted to call unimplemented virtual function: %s on type %s.",
            function, type->name);
    panic("Unimplemented virtual function trap triggered.");
}

bool AvmObjectEq(object lhs, object rhs) {
    AvmFunction method = AvmObjectType(lhs)->vptr[FUNC_EQ];

    size_t size = AvmObjectSize(lhs);

    if (method == NULL) {
        return memcmp(lhs, rhs, size) == 0;
    }

    return ((bool (*)(object, object))method)(lhs, rhs);
}

void AvmDestroy(object self) {
    AvmFunction method = AvmObjectType(self)->vptr[FUNC_DTOR];

    if (method == NULL) {
        free(self);
        return;
    }

    ((void (*)(object))method)(self);
}

size_t AvmGetLength(object self) {
    AvmFunction method = AvmObjectType(self)->vptr[FUNC_GET_LENGTH];

    if (method == NULL) {
        AvmVirtualFunctionTrap(__func__, AvmObjectType(self));
    }

    return ((size_t(*)(object))method)(self);
}

size_t AvmGetCapacity(object self) {
    AvmFunction method = AvmObjectType(self)->vptr[FUNC_GET_CAPACITY];

    if (method == NULL) {
        AvmVirtualFunctionTrap(__func__, AvmObjectType(self));
    }

    return ((size_t(*)(object))method)(self);
}

object AvmClone(object self) {
    AvmFunction method = AvmObjectType(self)->vptr[FUNC_CLONE];

    if (method == NULL) {
        size_t size = AvmObjectSize(self);
        return memcpy(malloc(size), self, size);
    }

    return ((object(*)(object))method)(self);
}

AvmString AvmToString(object self) {
    AvmFunction method = AvmObjectType(self)->vptr[FUNC_TO_STRING];

    if (method == NULL) {
        AvmVirtualFunctionTrap(__func__, AvmObjectType(self));
    }

    return ((object(*)(object))method)(self);
}

void AvmMemCopy(byte* source, size_t length, byte* destination, size_t size) {
    if (source == NULL) {
        panic(SourceNullMsg);
    }

    if (destination == NULL) {
        panic(DestNullMsg);
    }

    size_t trueLength = length > size ? size : length;
    memcpy(destination, source, trueLength);
}
