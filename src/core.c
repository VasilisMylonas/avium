#include "avium/core.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avium/fmt.h"
#include "avium/internal.h"

AvmType AvmObjectType(object self) {
    if (self == NULL) {
        panic("Parameter `self` was `NULL`.");
    }

    return *(AvmType*)self;
}

const char* AvmObjectName(object self) { return AvmObjectType(self)->name; }

size_t AvmObjectSize(object self) { return AvmObjectType(self)->size; }

never AvmPanic(const char* message, const char* function, const char* file,
               uint32_t line) {
    fprintf(stderr, "Panic in file %s:%u in function %s()\n%s\n", file, line,
            function, message);
    abort();
}

never AvmVirtualFunctionTrap(const char* function, AvmType type) {
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

AVMAPI AvmResult AvmSuccess(object value);
AVMAPI AvmResult AvmFailure(AvmErrorKind kind);
AVMAPI bool AvmIsFailure(AvmResult self);
AVMAPI object AvmUnwrap(AvmResult self);
AVMAPI AvmOptional AvmSome(object value);
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

struct _AvmVersion {
    AvmType type;
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
    char tag;
};

AvmString AvmVersionToString(AvmVersion self) {
    return AvmSprintf("%i.%i.%i-%c", self->major, self->minor, self->patch,
                      self->tag);
}

TYPE(AvmVersion, [FUNC_TO_STRING] = (AvmFunction)AvmVersionToString);

AvmVersion AvmVersion_ctor(uint32_t major, uint32_t minor, uint32_t patch,
                           char tag) {
    AvmVersion version = malloc(sizeof(struct _AvmVersion));
    version->type = GET_TYPE(AvmVersion);
    version->major = major;
    version->minor = minor;
    version->patch = patch;
    version->tag = tag;
    return version;
}

bool AvmVersionIsCompatible(AvmVersion self, AvmVersion other) {
    if (self == NULL) {
        panic(SelfNullMsg);
    }

    if (other == NULL) {
        panic(OtherNullMsg);
    }

    return self->major == other->major;
}

uint32_t AvmVersionGetMajor(AvmVersion self) {
    if (self == NULL) {
        panic(SelfNullMsg);
    }

    return self->major;
}

uint32_t AvmVersionGetMinor(AvmVersion self) {
    if (self == NULL) {
        panic(SelfNullMsg);
    }

    return self->minor;
}

uint32_t AvmVersionGetPatch(AvmVersion self) {
    if (self == NULL) {
        panic(SelfNullMsg);
    }

    return self->patch;
}

char AvmVersionGetTag(AvmVersion self) {
    if (self == NULL) {
        panic(SelfNullMsg);
    }

    return self->tag;
}
