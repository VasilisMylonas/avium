#include "avium/types.h"

#include <string.h>

#include "avium/core.h"
#include "avium/error.h"
#include "avium/private/resources.h"
#include "avium/string.h"
#include "avium/typeinfo.h"

const AvmType* AvmObjectGetType(object self)
{
    if (self == NULL)
    {
        AvmPanic(SelfNullMsg);
    }

    // The first member of an object should be a const AvmType*
    // Look in types.h for AVM_CLASS
    return *(const AvmType**)self;
}

bool AvmObjectEquals(object self, object other)
{
    if (self == NULL)
    {
        AvmPanic(SelfNullMsg);
    }

    if (other == NULL)
    {
        AvmPanic(OtherNullMsg);
    }

    const AvmType* type = AvmObjectGetType(self);
    AvmFunction fn = AvmTypeGetFunction(type, FnEntryEquals);
    uint size = type->_size;

    if (fn == NULL)
    {
        return memcmp(self, other, size) == 0;
    }

    return ((bool (*)(object, object))fn)(self, other);
}

void AvmObjectDestroy(object self)
{
    if (self == NULL)
    {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction fn = AvmTypeGetFunction(AvmObjectGetType(self), FnEntryDtor);

    if (fn != NULL)
    {
        ((void (*)(object))fn)(self);
    }
}

object AvmObjectClone(object self)
{
    if (self == NULL)
    {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction fn = AvmTypeGetFunction(AvmObjectGetType(self), FnEntryClone);

    if (fn == NULL)
    {
        uint size = AvmTypeGetSize(AvmObjectGetType(self));
        box(void) memory = AvmAlloc(size);
        AvmCopy(self, size, (byte*)memory);
        return memory;
    }

    return ((object(*)(object))fn)(self);
}

AvmString AvmObjectToString(object self)
{
    if (self == NULL)
    {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction fn =
        AvmTypeGetFunction(AvmObjectGetType(self), FnEntryToString);

    if (fn == NULL)
    {
        return AvmStringFormat(
            "%s [%x]", AvmTypeGetName(AvmObjectGetType(self)), self);
    }

    return ((AvmString(*)(object))fn)(self);
}

AVM_TYPE(object, object, {[FnEntryDtor] = NULL});
AVM_TYPE(_long, object, {[FnEntryDtor] = NULL});
AVM_TYPE(ulong, object, {[FnEntryDtor] = NULL});
AVM_TYPE(int, object, {[FnEntryDtor] = NULL});
AVM_TYPE(uint, object, {[FnEntryDtor] = NULL});
AVM_TYPE(short, object, {[FnEntryDtor] = NULL});
AVM_TYPE(ushort, object, {[FnEntryDtor] = NULL});
AVM_TYPE(char, object, {[FnEntryDtor] = NULL});
AVM_TYPE(byte, object, {[FnEntryDtor] = NULL});
