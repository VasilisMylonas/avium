#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avium/internal.h"

never AvmPanic(str message, str function, str file, uint line) {
    fprintf(stderr, "Panic in file %s:%u in function %s()\n%s\n", file, line,
            function, message);
    abort();
}

AvmResult AvmSuccess(object value);
AvmResult AvmFailure(AvmErrorKind kind, str error);
str AvmResultGetError(AvmResult self);
bool AvmResultIsFailure(AvmResult self);
object AvmResultUnwrap(AvmResult self);
AvmOptional AvmSome(object value);
AvmOptional AvmNone();
bool AvmOptionalHasValue(AvmOptional self);

// AvmType struct definition is in internal.h

str AvmTypeName(AvmType self) {
    if (self == NULL) {
        panic(SelfNullMsg);
    }

    return self->name;
}

size_t AvmTypeSize(AvmType self) {
    if (self == NULL) {
        panic(SelfNullMsg);
    }

    return self->size;
}

AvmType AvmObjectType(object self) {
    if (self == NULL) {
        panic(SelfNullMsg);
    }

    return *(AvmType*)self;
}

bool AvmObjectEquals(object lhs, object rhs) {
    AvmType type = AvmObjectType(lhs);
    AvmFunction method = type->vptr[FUNC_EQ];

    size_t size = type->size;

    if (method == NULL) {
        return memcmp(lhs, rhs, size) == 0;
    }

    return ((bool (*)(object, object))method)(lhs, rhs);
}

void AvmObjectDestroy(object self) {
    AvmFunction method = AvmObjectType(self)->vptr[FUNC_DTOR];

    if (method == NULL) {
        free(self);
        return;
    }

    ((void (*)(object))method)(self);
}

object AvmObjectClone(object self) {
    AvmFunction method = AvmObjectType(self)->vptr[FUNC_CLONE];

    if (method == NULL) {
        size_t size = AvmTypeSize(AvmObjectType(self));
        return memcpy(malloc(size), self, size);
    }

    return ((object(*)(object))method)(self);
}

AvmString AvmObjectToString(object self) {
    AvmFunction method = AvmObjectType(self)->vptr[FUNC_TO_STRING];

    if (method == NULL) {
        AvmVirtualFunctionTrap(__func__, AvmObjectType(self));
    }

    return ((void* (*)(object))method)(self);
}

never AvmVirtualFunctionTrap(str function, AvmType type) {
    fprintf(stderr,
            "Attempted to call unimplemented virtual function: %s on type %s.",
            function, type->name);
    panic("Unimplemented virtual function trap triggered.");
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