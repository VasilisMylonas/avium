#include "avium/version.h"

#include "avium/runtime.h"    // For AVM_TYPE
#include "avium/fmt.h"        // For AvmSprintf
#include "avium/resources.h"  // For VersionFormat

static AvmString AvmVersionToString(AvmVersion* self) {
    return AvmSprintf(VersionFormat, self->Major, self->Minor, self->Patch,
                      self->Tag);
}

AVM_TYPE(AvmVersion, object,
         {[FnEntryToString] = (AvmFunction)AvmVersionToString});

AvmVersion AvmVersionFrom(ushort major, ushort minor, ushort patch, char tag) {
    return (AvmVersion){
        ._type = typeid(AvmVersion),
        .Major = major,
        .Minor = minor,
        .Patch = patch,
        .Tag = tag,
    };
}

AvmVersion AvmRuntimeGetVersion(void) {
    return AvmVersionFrom(AVM_VERSION_MAJOR, AVM_VERSION_MINOR,
                          AVM_VERSION_PATCH, 'd');
}
