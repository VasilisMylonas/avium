#include "avium/version.h"

#include "avium/core.h"              // For typeid
#include "avium/private/resources.h" // For VersionFormat
#include "avium/string.h"            // For AvmStringFormat
#include "avium/typeinfo.h"

static AvmString AvmVersionToString(AvmVersion* self)
{
    return AvmStringFormat("%i.%i.%i", self->Major, self->Minor, self->Patch);
}

AVM_TYPE(AvmVersion,
         object,
         {[FnEntryToString] = (AvmFunction)AvmVersionToString});

AvmVersion AvmVersionFrom(ushort major, ushort minor, ushort patch)
{
    return (AvmVersion){
        ._type = typeid(AvmVersion),
        .Major = major,
        .Minor = minor,
        .Patch = patch,
    };
}

AvmVersion AvmRuntimeGetVersion(void)
{
    return AvmVersionFrom(
        AVM_VERSION_MAJOR, AVM_VERSION_MINOR, AVM_VERSION_PATCH);
}
