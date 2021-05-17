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

AVMAPI AvmArrayList AvmArrayListNew(const AvmType* type, uint length);
AVMAPI uint AvmArrayListGetLength(AvmArrayList* self);
AVMAPI uint AvmArrayListGetCapacity(AvmArrayList* self);
AVMAPI const AvmType* AvmArrayListGetItemType(AvmArrayList* self);
AVMAPI AvmError* AvmArrayListInsert(AvmArrayList* self,
                                    uint index,
                                    object value);

AVMAPI AvmError* AvmArrayListRemove(AvmArrayList* self, uint index);
AVMAPI object AvmArrayListItemAt(AvmArrayList* self,
                                 uint index,
                                 AvmError** error);

AVMAPI void AvmArrayListClear(AvmArrayList* self);
AVMAPI AvmError* AvmArrayListPush(AvmArrayList* self, object value);
AVMAPI void AvmArrayListErase(AvmArrayList* self);

#endif // AVIUM_COLLECTIONS_ARRAY_LIST_H
