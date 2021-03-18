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

str AvmObjectName(object self) { return AvmObjectType(self)->name; }

size_t AvmObjectSize(object self) { return AvmObjectType(self)->size; }

never AvmPanic(str message, str function, str file, uint line) {
    fprintf(stderr, "Panic in file %s:%u in function %s()\n%s\n", file, line,
            function, message);
    abort();
}

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

AVMAPI AvmResult AvmSuccess(object value);
AVMAPI AvmResult AvmFailure(AvmErrorKind kind);
AVMAPI bool AvmIsFailure(AvmResult self);
AVMAPI object AvmUnwrap(AvmResult self);
AVMAPI AvmOptional AvmSome(object value);
AVMAPI AvmOptional AvmNone();
AVMAPI bool AvmHasValue(AvmOptional optional);

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

struct AvmVersion {
    AvmType type;
    uint major;
    uint minor;
    uint patch;
    char tag;
};

AvmString AvmVersionToString(AvmVersion self) {
    return AvmSprintf("%i.%i.%i-%c", self->major, self->minor, self->patch,
                      self->tag);
}

TYPE(AvmVersion, [FUNC_TO_STRING] = (AvmFunction)AvmVersionToString);

AvmVersion AvmVersion_ctor(uint major, uint minor, uint patch, char tag) {
    AvmVersion version = malloc(sizeof(struct AvmVersion));
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

uint AvmVersionGetMajor(AvmVersion self) {
    if (self == NULL) {
        panic(SelfNullMsg);
    }

    return self->major;
}

uint AvmVersionGetMinor(AvmVersion self) {
    if (self == NULL) {
        panic(SelfNullMsg);
    }

    return self->minor;
}

uint AvmVersionGetPatch(AvmVersion self) {
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

static inline bool IsLongOption(str arg) {
    return arg[0] == '-' && arg[1] == '-' && arg[2] != '-';
}

static inline bool IsShortOption(str arg) {
    return arg[0] == '-' && arg[1] != '-';
}

static bool IsOption(str arg, char shortOption, str longOption) {
    if (shortOption != 0 && IsShortOption(arg)) {
        return arg[1] == shortOption;
    }

    if (longOption != NULL && IsLongOption(arg)) {
        return strncmp(arg + 2, longOption, strlen(longOption)) == 0;
    }

    return false;
}

static bool OptionHasArgument(str arg, size_t length) {
    size_t offset = IsLongOption(arg) ? 2 : 1;

    return arg[offset + length] == '=' && arg[offset + length + 1] != '\0';
}

static str OptionGetArgument(str arg, size_t length) {
    size_t offset = IsLongOption(arg) ? 2 : 1;
    return arg + offset + length + 1;
}

bool AvmHasOption(int argc, str* argv, AvmOption option) {
    for (int i = 0; i < argc; i++) {
        if (IsOption(argv[i], option.shortOption, option.longOption)) {
            return true;
        }
    }

    return false;
}

AvmOptional AvmGetOption(int argc, str* argv, AvmOption option) {
    const size_t length = strlen(option.longOption);

    for (int i = 1; i < argc; i++) {
        if (IsOption(argv[i], option.shortOption, option.longOption)) {
            if (OptionHasArgument(argv[i], length)) {
                return AvmSome((object)OptionGetArgument(argv[i], length));
            } else {
                return AvmSome((object) "");
            }
        }
    }

    return AvmNone();
}
