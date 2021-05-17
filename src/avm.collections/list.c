#include "avium/collections/list.h"

#include "avium/runtime.h"
#include "avium/testing.h"

uint AvmListGetLength(AvmList* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmFunction fn =
        AvmTypeGetFunction(AvmObjectGetType(self), FnEntryGetLength);
    return ((uint(*)(AvmList*))fn)(self);
}

uint AvmListGetCapacity(AvmList* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmFunction fn =
        AvmTypeGetFunction(AvmObjectGetType(self), FnEntryGetCapacity);
    return ((uint(*)(AvmList*))fn)(self);
}

const AvmType* AvmListGetItemType(AvmList* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmFunction fn =
        AvmTypeGetFunction(AvmObjectGetType(self), FnEntryGetItemType);
    return ((const AvmType* (*)(AvmList*))fn)(self);
}

AvmError* AvmListInsert(AvmList* self, uint index, object value)
{
    pre
    {
        assert(self != NULL);
        assert(value != NULL);
    }

    AvmFunction fn = AvmTypeGetFunction(AvmObjectGetType(self), FnEntryInsert);
    return ((AvmError * (*)(AvmList*, uint, object)) fn)(self, index, value);
}

AvmError* AvmListRemove(AvmList* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    AvmFunction fn = AvmTypeGetFunction(AvmObjectGetType(self), FnEntryRemove);
    return ((AvmError * (*)(AvmList*, uint)) fn)(self, index);
}

object AvmListItemAt(AvmList* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    AvmFunction fn = AvmTypeGetFunction(AvmObjectGetType(self), FnEntryItemAt);
    return ((object(*)(AvmList*, uint))fn)(self, index);
}

void AvmListClear(AvmList* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmFunction fn = AvmTypeGetFunction(AvmObjectGetType(self), FnEntryItemAt);
    ((void (*)(AvmList*))fn)(self);
}

AvmError* AvmListPush(AvmList* self, object value)
{
    pre
    {
        assert(self != NULL);
        assert(value != NULL);
    }

    return AvmListInsert(self, AvmListGetLength(self), value);
}

object AvmListPop(AvmList* self)
{
    pre
    {
        assert(self != NULL);
    }

    const uint length = AvmListGetLength(self);
    object item = AvmObjectClone(AvmListItemAt(self, length - 1));
    AvmListRemove(self, length - 1);
    return item;
}

bool AvmListContains(AvmList* self, object value)
{
    pre
    {
        assert(self != NULL);
        assert(value != NULL);
    }

    const uint length = AvmListGetLength(self);
    for (uint i = 0; i < length; i++)
    {
        object temp = AvmListItemAt(self, i);
        if (AvmObjectEquals(temp, value))
        {
            return true;
        }
    }

    return false;
}

uint AvmListIndexOf(AvmList* self, object value)
{
    pre
    {
        assert(self != NULL);
        assert(value != NULL);
    }

    const uint length = AvmListGetLength(self);
    for (uint i = 0; i < length; i++)
    {
        object temp = AvmListItemAt(self, i);
        if (AvmObjectEquals(temp, value))
        {
            return i;
        }
    }

    return AvmInvalid;
}
