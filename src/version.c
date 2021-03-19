#include "avium/version.h"
#include "avium/internal.h"
#include "avium/string.h"
#include "avium/object.h"
#include "avium/fmt.h"

#include <stdlib.h>

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
