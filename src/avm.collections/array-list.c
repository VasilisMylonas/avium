#include "avium/collections/array-list.h"

#include "avium/collections/list.h"
#include "avium/private/constants.h"
#include "avium/string.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

#include <string.h>

static uint AvmArrayListGetLength(const AvmArrayList* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_length;
}

static uint AvmArrayListGetCapacity(const AvmArrayList* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_capacity;
}

static const AvmType* AvmArrayListGetItemType(const AvmArrayList* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_itemType;
}

static object AvmArrayListItemAt(const AvmArrayList* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    if (index >= self->_length)
    {
        throw(AvmErrorNew(RangeError));
    }

    return self->_items + index * self->_itemType->_size;
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

static void AvmArrayListInsert(AvmArrayList* self, uint index, object value)
{
    pre
    {
        assert(self != NULL);
        assert(value != NULL);
    }

    if (index > self->_length)
    {
        throw(AvmErrorNew(RangeError));
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
}

static void AvmArrayListDropItem(AvmArrayList* self, uint index)
{
    AvmFunction fn = AvmTypeGetFunction(self->_itemType, FnEntryFinalize);
    object item = AvmArrayListItemAt(self, index);

    if (fn != NULL)
    {
        ((void (*)(object))fn)(item);
    }
}

static void AvmArrayListRemove(AvmArrayList* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    if (index >= self->_length)
    {
        throw(AvmErrorNew(RangeError));
    }

    AvmArrayListDropItem(self, index);
    self->_length--;

    // If we remove the last item then no need to compact.
    if (index == self->_length)
    {
        return;
    }

    const size_t itemSize = self->_itemType->_size;
    const size_t freeSize = (self->_length - index) * itemSize;
    byte* const objectPtr = self->_items + index * itemSize;

    memmove(objectPtr, objectPtr + itemSize, freeSize); // Shift elements in.
}

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
        object item = AvmArrayListItemAt(self, i);

        if (self->_itemType->_size > sizeof(AvmType*)) // Type is not primitive.
            AvmStringPushValue(&s, AvmArrayListItemAt(self, i));
        else if (self->_itemType == typeid(float))
            AvmStringPushFloat(&s, *(float*)item, FloatReprAuto);
        else if (self->_itemType == typeid(double))
            AvmStringPushFloat(&s, *(double*)item, FloatReprAuto);
        else if (self->_itemType == typeid(str))
            AvmStringPushStr(&s, *(str*)item);
        else if (self->_itemType == typeid(byte))
            AvmStringPushUint(&s, *(byte*)item, NumericBaseDecimal);
        else if (self->_itemType == typeid(ushort))
            AvmStringPushUint(&s, *(ushort*)item, NumericBaseDecimal);
        else if (self->_itemType == typeid(uint))
            AvmStringPushUint(&s, *(uint*)item, NumericBaseDecimal);
        else if (self->_itemType == typeid(ulong))
            AvmStringPushUint(&s, *(ulong*)item, NumericBaseDecimal);
        else if (self->_itemType == typeid(char))
            AvmStringPushChar(&s, *(char*)item);
        else if (self->_itemType == typeid(short))
            AvmStringPushInt(&s, *(short*)item);
        else if (self->_itemType == typeid(int))
            AvmStringPushInt(&s, *(int*)item);
        else if (self->_itemType == typeid(_long))
            AvmStringPushInt(&s, *(_long*)item);
        else
            throw(AvmErrorNew(InternalError));

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
