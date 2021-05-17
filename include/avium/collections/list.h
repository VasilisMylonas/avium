#ifndef AVIUM_COLLECTIONS_LIST_H
#define AVIUM_COLLECTIONS_LIST_H

#include "avium/error.h"
#include "avium/types.h"

AVM_INTERFACE(AvmList);

AVMAPI uint AvmListGetLength(AvmList* self);
AVMAPI uint AvmListGetCapacity(AvmList* self);
AVMAPI const AvmType* AvmListGetItemType(AvmList* self);

AVMAPI AvmError* AvmListInsert(AvmList* self, uint index, object value);
AVMAPI AvmError* AvmListRemove(AvmList* self, uint index);
AVMAPI object AvmListItemAt(AvmList* self, uint index);
AVMAPI void AvmListClear(AvmList* self);

AVMAPI AvmError* AvmListPush(AvmList* self, object value);
AVMAPI object AvmListPop(AvmList* self);
AVMAPI bool AvmListContains(AvmList* self, object value);
AVMAPI uint AvmListIndexOf(AvmList* self, object value);

#endif // AVIUM_COLLECTIONS_LIST_H
