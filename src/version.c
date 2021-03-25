#include "avium/version.h"
#include "avium/internal.h"
#include "avium/string.h"
#include "avium/fmt.h"

#include <stdlib.h>

AvmString AvmVersionToString(AvmVersion* self) {
    return AvmSprintf("%i.%i.%i-%c", self->major, self->minor, self->patch,
                      self->tag);
}

AVM_TYPE(AvmVersion, {[FUNC_TO_STRING] = (AvmFunction)AvmVersionToString});

AvmVersion AvmVersionFrom(ushort major, ushort minor, ushort patch, char tag) {
    return (AvmVersion){
        ._type = AVM_GET_TYPE(AvmVersion),
        .major = major,
        .minor = minor,
        .patch = patch,
        .tag = tag,
    };
}

bool AvmVersionIsCompatible(AvmVersion* self, AvmVersion* other) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (other == NULL) {
        AvmPanic(OtherNullMsg);
    }

    return self->major == other->major;
}
