#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "avium/prologue.h"
#include "avium/resources.h"
#include "avium/fmt.h"

static void ExceptionHandler(int exception) {
    switch (exception) {
        case SIGSEGV:
            AvmPanic("Invalid pointer dereference.");
            break;
        case SIGILL:
            AvmPanic("Illegal instruction.");
            break;
        case SIGFPE:
            AvmPanic("Arithmetic exception.");
            break;
        case SIGINT:
            AvmPanic("Received interrupt.");
            break;
        default:
            break;
    }
}

void AvmEnableExceptions(void) {
    signal(SIGSEGV, ExceptionHandler);
    signal(SIGILL, ExceptionHandler);
    signal(SIGFPE, ExceptionHandler);
    signal(SIGINT, ExceptionHandler);
}

void AvmDisableExceptions(void) {
    signal(SIGSEGV, SIG_DFL);
    signal(SIGILL, SIG_DFL);
    signal(SIGFPE, SIG_DFL);
    signal(SIGINT, SIG_DFL);
}

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
        AvmVirtualFunctionTrap();
    }

    return ((AvmString(*)(object))method)(self);
}

void AvmObjectCopy(object self, size_t size, byte buffer[]) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (buffer == NULL) {
        AvmPanic(BufferNullMsg);
    }

    size_t objectSize = AvmTypeGetSize(AvmObjectGetType(self));
    AvmMemCopy(self, objectSize, buffer, size);
}

never AvmVirtualFunctionTrap(void) { AvmPanic(VirtualFuncTrapTriggered); }

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

static AvmString AvmVersionToString(AvmVersion* self) {
    return AvmSprintf(VersionFormat, self->Major, self->Minor, self->Patch,
                      self->Tag);
}

AVM_TYPE(AvmVersion, {[FUNC_TO_STRING] = (AvmFunction)AvmVersionToString});

AvmVersion AvmVersionFrom(ushort major, ushort minor, ushort patch, char tag) {
    return (AvmVersion){
        ._type = AVM_GET_TYPE(AvmVersion),
        .Major = major,
        .Minor = minor,
        .Patch = patch,
        .Tag = tag,
    };
}
