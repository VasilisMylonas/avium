#include "avium/collections/list.h"

#include "avium/private/resources.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

uint AvmListGetLength(const AvmList* self)
{
    VIRTUAL_CALL(uint, FnEntryGetLength, self);
}

uint AvmListGetCapacity(const AvmList* self)
{
    VIRTUAL_CALL(uint, FnEntryGetCapacity, self);
}

const AvmType* AvmListGetItemType(const AvmList* self)
{
    VIRTUAL_CALL(const AvmType*, FnEntryInsert, self);
}

object AvmListItemAt(const AvmList* self, uint index)
{
    VIRTUAL_CALL(object, FnEntryItemAt, self, index);
}

void AvmListInsert(AvmList* self, uint index, object value)
{
    VIRTUAL_CALL(void, FnEntryInsert, self, index, value);
}

void AvmListRemove(AvmList* self, uint index)
{
    VIRTUAL_CALL(void, FnEntryRemove, self, index);
}

void AvmListClear(AvmList* self)
{
    pre
    {
        assert(self != NULL);
    }

    uint length = AvmListGetLength(self);

    for (uint i = length; i > 1; i--)
    {
        AvmListRemove(self, i - 1);
    }
}

void AvmListPush(AvmList* self, object value)
{
    AvmListInsert(self, AvmListGetLength(self), value);
}

object AvmListPop(AvmList* self)
{
    const uint length = AvmListGetLength(self);
    object item = AvmObjectClone(AvmListItemAt(self, length - 1));
    AvmListRemove(self, length - 1);
    return item;
}

bool AvmListContains(const AvmList* self, object value)
{
    return AvmListIndexOf(self, value) != AvmInvalid;
}

uint AvmListIndexOf(const AvmList* self, object value)
{
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
