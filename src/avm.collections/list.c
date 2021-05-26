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

AvmError* AvmListInsert(AvmList* self, uint index, object value)
{
    VIRTUAL_CALL(AvmError*, FnEntryInsert, self, index, value);
}

AvmError* AvmListRemove(AvmList* self, uint index)
{
    VIRTUAL_CALL(AvmError*, FnEntryRemove, self, index);
}

void AvmListClear(AvmList* self)
{
    VIRTUAL_CALL(void, FnEntryClear, self);
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

bool AvmListContains(const AvmList* self, object value)
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

uint AvmListIndexOf(const AvmList* self, object value)
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
