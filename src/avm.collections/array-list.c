#include "avium/collections/array-list.h"

#include "avium/string.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

#include <string.h>

static AvmString AvmArrayListToString(AvmArrayList* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmString s = AvmStringNew(self->_length * 2);

    AvmStringPushStr(&s, "[ ");
    for (uint i = 0; i < self->_length; i++)
    {
        AvmStringPushValue(&s, AvmArrayListItemAt(self, i));
        if (i < self->_length - 1)
        {
            AvmStringPushStr(&s, ", ");
        }
    }
    AvmStringPushStr(&s, " ]");
    return s;
}

AVM_TYPE(AvmArrayList,
         object,
         {
             [FnEntryGetLength] = (AvmFunction)AvmArrayListGetLength,
             [FnEntryGetCapacity] = (AvmFunction)AvmArrayListGetCapacity,
             [FnEntryGetItemType] = (AvmFunction)AvmArrayListGetItemType,
             [FnEntryInsert] = (AvmFunction)AvmArrayListInsert,
             [FnEntryRemove] = (AvmFunction)AvmArrayListRemove,
             [FnEntryItemAt] = (AvmFunction)AvmArrayListItemAt,
             [FnEntryClear] = (AvmFunction)AvmArrayListClear,
             [FnEntryToString] = (AvmFunction)AvmArrayListToString,
         });

AvmArrayList AvmArrayListNew(const AvmType* type, uint capacity)
{
    pre
    {
        assert(type != NULL);
    }

    return (AvmArrayList){
        ._type = typeid(AvmArrayList),
        ._itemType = type,
        ._length = 0,
        ._items = capacity == 0 ? NULL : AvmAlloc(capacity * type->_size),
        ._capacity = capacity,
    };
}

uint AvmArrayListGetLength(const AvmArrayList* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_length;
}

uint AvmArrayListGetCapacity(const AvmArrayList* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_capacity;
}

const AvmType* AvmArrayListGetItemType(const AvmArrayList* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_itemType;
}

// The following code is straight up copied from avm.core/string.c with minor
// modifications.
static void AvmArrayListEnsureCapacity(AvmArrayList* self, uint capacity)
{
    pre
    {
        assert(self != NULL);
    }

    if (capacity == 0)
    {
        return;
    }

    const size_t totalRequired = self->_length + capacity;
    const size_t newCapacity = self->_capacity * AVM_ARRAY_LIST_GROWTH_FACTOR;

    if (totalRequired > self->_capacity)
    {
        // TODO: There may be more efficient ways of doing this
        self->_capacity = newCapacity;

        if (newCapacity < totalRequired)
        {
            self->_capacity += capacity;
        }

        self->_items =
            AvmRealloc(self->_items, self->_capacity * self->_itemType->_size);
    }

    post
    {
        assert(self->_capacity >= self->_length);
        assert(self->_capacity >= capacity);
    }
}

AvmError* AvmArrayListInsert(AvmArrayList* self, uint index, object value)
{
    pre
    {
        assert(self != NULL);
        assert(value != NULL);
    }

    if (index > self->_length)
    {
        return AvmErrorOfKind(ErrorKindRange);
    }

    AvmArrayListEnsureCapacity(self, 1);

    const size_t size = self->_itemType->_size;
    byte* const dest = self->_items + index * size;

    if (index < self->_length)
    {
        memmove(dest + size, dest, size); // Shift all elements out.
    }

    memcpy(dest, value, size);
    self->_length++;
    return NULL;
}

AvmError* AvmArrayListPush(AvmArrayList* self, object value)
{
    pre
    {
        assert(self != NULL);
        assert(value != NULL);
    }

    return AvmArrayListInsert(self, self->_length, value);
}

AvmError* AvmArrayListRemove(AvmArrayList* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    if (index >= self->_length)
    {
        return AvmErrorOfKind(ErrorKindRange);
    }

    const size_t size = self->_itemType->_size;
    byte* const ptr = self->_items + index * size;
    memmove(ptr, ptr + size, size); // Shift elements in.
    return NULL;
}

object AvmArrayListItemAt(AvmArrayList* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    if (index >= self->_length)
    {
        // TODO
        AvmPanic("TODO");
    }

    return self->_items + index * self->_itemType->_size;
}

void AvmArrayListClear(AvmArrayList* self)
{
    pre
    {
        assert(self != NULL);
    }

    self->_length = 0;
}

void AvmArrayListErase(AvmArrayList* self)
{
    pre
    {
        assert(self != NULL);
    }

    memset(self->_items, 0, self->_length * self->_itemType->_size);
    self->_length = 0;
}
