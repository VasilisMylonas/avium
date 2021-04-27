#include "avium/types.h"

#include <string.h>  // For memcmp

#include "avium/runtime.h"
#include "avium/private/resources.h"
#include "avium/string.h"

AvmType* AvmObjectGetType(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    // The first member of an object should be an AvmType*
    // Look in types.h for AVM_CLASS
    return *(AvmType**)self;
}

bool AvmObjectEquals(object self, object other) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (other == NULL) {
        AvmPanic(OtherNullMsg);
    }

    AvmType* type = AvmObjectGetType(self);
    AvmFunction fn = AvmTypeGetFunction(type, FnEntryEquals);
    size_t size = type->_size;

    if (fn == NULL) {
        return memcmp(self, other, size) == 0;
    }

    return ((bool (*)(object, object))fn)(self, other);
}

void AvmObjectDestroy(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction fn = AvmTypeGetFunction(AvmObjectGetType(self), FnEntryDtor);

    if (fn != NULL) {
        ((void (*)(object))fn)(self);
    }
}

object AvmObjectClone(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction fn = AvmTypeGetFunction(AvmObjectGetType(self), FnEntryClone);

    if (fn == NULL) {
        size_t size = AvmTypeGetSize(AvmObjectGetType(self));
        void* memory = AvmAlloc(size);
        AvmMemCopy((byte*)self, size, (byte*)memory, size);
        return memory;
    }

    return ((object(*)(object))fn)(self);
}

AvmString AvmObjectToString(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction fn =
        AvmTypeGetFunction(AvmObjectGetType(self), FnEntryToString);

    if (fn == NULL) {
        return AvmStringFormat("%s [%x]",
                               AvmTypeGetName(AvmObjectGetType(self)), self);
    }

    return ((AvmString(*)(object))fn)(self);
}

AVM_TYPE(size_t, {[FnEntryDtor] = NULL});
AVM_TYPE(_long, {[FnEntryDtor] = NULL});
AVM_TYPE(ulong, {[FnEntryDtor] = NULL});
AVM_TYPE(int, {[FnEntryDtor] = NULL});
AVM_TYPE(uint, {[FnEntryDtor] = NULL});
AVM_TYPE(short, {[FnEntryDtor] = NULL});
AVM_TYPE(ushort, {[FnEntryDtor] = NULL});
AVM_TYPE(char, {[FnEntryDtor] = NULL});
AVM_TYPE(byte, {[FnEntryDtor] = NULL});