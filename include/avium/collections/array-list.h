#ifndef AVIUM_COLLECTIONS_ARRAY_LIST_H
#define AVIUM_COLLECTIONS_ARRAY_LIST_H

#include "avium/types.h"

AVM_CLASS(AvmArrayList, object, {
    uint _length;
    uint _capacity;
    const AvmType* _itemType;
    byte* _items;
});

AVMAPI AvmArrayList AvmArrayListNew(const AvmType* type, uint length);

#endif // AVIUM_COLLECTIONS_ARRAY_LIST_H
