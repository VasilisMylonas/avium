#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avium/internal.h"

// AvmType struct definition is in internal.h

str AvmTypeGetName(const AvmType* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_name;
}

size_t AvmTypeGetSize(const AvmType* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_size;
}

const AvmType* AvmObjectGetType(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return *(AvmType**)self;
}

bool AvmObjectEquals(object lhs, object rhs) {
    const AvmType* type = AvmObjectGetType(lhs);
    AvmFunction method = type->_vptr[FUNC_EQ];

    size_t size = type->_size;

    if (method == NULL) {
        return memcmp(lhs, rhs, size) == 0;
    }

    return ((bool (*)(object, object))method)(lhs, rhs);
}

void AvmObjectDestroy(object self) {
    AvmFunction method = AvmObjectGetType(self)->_vptr[FUNC_DTOR];

    if (method == NULL) {
        free(self);
        return;
    }

    ((void (*)(object))method)(self);
}

object AvmObjectClone(object self) {
    AvmFunction method = AvmObjectGetType(self)->_vptr[FUNC_CLONE];

    if (method == NULL) {
        size_t size = AvmTypeGetSize(AvmObjectGetType(self));
        return memcpy(malloc(size), self, size);
    }

    return ((object(*)(object))method)(self);
}

AvmString AvmObjectToString(object self) {
    AvmFunction method = AvmObjectGetType(self)->_vptr[FUNC_TO_STRING];

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

never AvmVirtualFunctionTrap(str function, const AvmType* type) {
    fprintf(stderr,
            "Attempted to call unimplemented virtual function: %s on type %s.",
            function, type->_name);
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

#ifdef AVM_LINUX
#    include <execinfo.h>
#endif

never AvmPanicEx(str message, str function, str file, uint line) {
    fprintf(stderr, "Panic in file %s:%u in function %s()\n\n%s\n", file, line,
            function, message);

#ifdef AVM_LINUX
    object arr[128];

    int length = backtrace(arr, 128);
    char** s = backtrace_symbols(arr, length);

    for (int i = length - 1; i >= 1; i--) {
        *(strrchr(s[i], ')')) = '\0';
        fprintf(stderr, "    at %s\n", strchr(s[i], '(') + 1);
    }
#else
    fprintf(stderr, "No backtrace is available.\n");
#endif

    exit(1);
}
