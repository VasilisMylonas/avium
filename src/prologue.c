#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avium/internal.h"

// AvmType struct definition is in internal.h

str AvmTypeGetName(AvmType self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->name;
}

size_t AvmTypeGetSize(AvmType self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->size;
}

AvmType AvmObjectGetType(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return *(AvmType*)self;
}

bool AvmObjectEquals(object lhs, object rhs) {
    AvmType type = AvmObjectGetType(lhs);
    AvmFunction method = type->vptr[FUNC_EQ];

    size_t size = type->size;

    if (method == NULL) {
        return memcmp(lhs, rhs, size) == 0;
    }

    return ((bool (*)(object, object))method)(lhs, rhs);
}

void AvmObjectDestroy(object self) {
    AvmFunction method = AvmObjectGetType(self)->vptr[FUNC_DTOR];

    if (method == NULL) {
        free(self);
        return;
    }

    ((void (*)(object))method)(self);
}

object AvmObjectClone(object self) {
    AvmFunction method = AvmObjectGetType(self)->vptr[FUNC_CLONE];

    if (method == NULL) {
        size_t size = AvmTypeGetSize(AvmObjectGetType(self));
        return memcpy(malloc(size), self, size);
    }

    return ((object(*)(object))method)(self);
}

AvmString AvmObjectToString(object self) {
    AvmFunction method = AvmObjectGetType(self)->vptr[FUNC_TO_STRING];

    if (method == NULL) {
        AvmVirtualFunctionTrap(__func__, AvmObjectGetType(self));
    }

    return ((AvmString(*)(object))method)(self);
}

void AvmObjectCopy(object self, size_t size, byte buffer[]) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (buffer == NULL) {
        AvmPanic("Parameter `buffer` is `NULL`.");
    }

    size_t objectSize = AvmTypeGetSize(AvmObjectGetType(self));
    AvmMemCopy(self, objectSize, buffer, size);
}

never AvmVirtualFunctionTrap(str function, AvmType type) {
    fprintf(stderr,
            "Attempted to call unimplemented virtual function: %s on type %s.",
            function, type->name);
    AvmPanic("Unimplemented virtual function trap triggered.");
}

void AvmMemCopy(byte* source, size_t length, byte* destination, size_t size) {
    if (source == NULL) {
        AvmPanic(SourceNullMsg);
    }

    if (destination == NULL) {
        AvmPanic(DestNullMsg);
    }

    size_t trueLength = length > size ? size : length;
    memcpy(destination, source, trueLength);
}

never AvmPanicEx(str message, str function, str file, uint line) {
    fprintf(stderr, "Panic in file %s:%u in function %s()\n%s\n", file, line,
            function, message);
    abort();
}
