#include <string.h>
#include <stdio.h>   // For fprintf
#include <stdlib.h>  // For exit

#include "avium/types.h"
#include "avium/typeinfo.h"
#include "avium/resources.h"
#include "avium/runtime.h"

const AvmType* AvmObjectGetType(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return *(AvmType**)self;
}

bool AvmObjectEquals(object self, object other) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (other == NULL) {
        AvmPanic(OtherNullMsg);
    }

    const AvmType* type = AvmObjectGetType(self);
    AvmFunction method = type->_vptr[FUNC_EQUALS];
    size_t size = type->_size;

    if (method == NULL) {
        return memcmp(self, other, size) == 0;
    }

    return ((bool (*)(object, object))method)(self, other);
}

void AvmObjectDestroy(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction method = AvmObjectGetType(self)->_vptr[FUNC_DTOR];

    if (method != NULL) {
        ((void (*)(object))method)(self);
    }
}

object AvmObjectClone(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction method = AvmObjectGetType(self)->_vptr[FUNC_CLONE];

    if (method == NULL) {
        size_t size = AvmTypeGetSize(AvmObjectGetType(self));
        return memcpy(malloc(size), self, size);
    }

    return ((object(*)(object))method)(self);
}

AvmString AvmObjectToString(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction method = AvmObjectGetType(self)->_vptr[FUNC_TO_STRING];

    if (method == NULL) {
        AvmRuntimeVirtualFunctionTrap();
    }

    return ((AvmString(*)(object))method)(self);
}

void AvmMemCopy(byte* source, size_t length, byte* destination, size_t size) {
    if (source == NULL) {
        AvmPanic(SourceNullMsg);
    }

    if (destination == NULL) {
        AvmPanic(DestinationNullMsg);
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
