#include "avium/collections/list.h"

#include "avium/testing.h"
#include "avium/typeinfo.h"

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
