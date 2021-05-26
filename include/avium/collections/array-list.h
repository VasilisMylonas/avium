#ifndef AVIUM_COLLECTIONS_ARRAY_LIST_H
#define AVIUM_COLLECTIONS_ARRAY_LIST_H

#include "avium/error.h"
#include "avium/types.h"

AVM_CLASS(AvmArrayList, object, {
    uint _length;
    uint _capacity;
    const AvmType* _itemType;
    byte* _items;
});

AVMAPI uint AvmArrayListGetLength(const AvmArrayList* self);
AVMAPI uint AvmArrayListGetCapacity(const AvmArrayList* self);
AVMAPI const AvmType* AvmArrayListGetItemType(const AvmArrayList* self);

AVMAPI AvmError* AvmArrayListInsert(AvmArrayList* self,
                                    uint index,
                                    object value);
AVMAPI AvmError* AvmArrayListRemove(AvmArrayList* self, uint index);
AVMAPI void AvmArrayListClear(AvmArrayList* self);
AVMAPI AvmError* AvmArrayListPush(AvmArrayList* self, object value);

AVMAPI AvmArrayList AvmArrayListNew(const AvmType* type, uint length);

AVMAPI object AvmArrayListItemAt(AvmArrayList* self, uint index);

AVMAPI void AvmArrayListErase(AvmArrayList* self);

#endif // AVIUM_COLLECTIONS_ARRAY_LIST_H
