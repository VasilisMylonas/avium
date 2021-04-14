#include "avium/types.h"

#include <string.h>  // For memcmp

#include "avium/runtime.h"
#include "avium/resources.h"
#include "avium/fmt.h"

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

    const AvmType* type = AvmObjectGetType(self);
    AvmFunction method = type->_vptr[FnEntryEquals];
    size_t size = type->_size;

    if (method == NULL) {
        return memcmp(self, other, size) == 0;
    }

    return ((bool (*)(object, object))method)(self, other);
}

void AvmObjectDestroy(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction method = AvmObjectGetType(self)->_vptr[FnEntryDtor];

    if (method != NULL) {
        ((void (*)(object))method)(self);
    }
}

object AvmObjectClone(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction method = AvmObjectGetType(self)->_vptr[FnEntryClone];

    if (method == NULL) {
        size_t size = AvmTypeGetSize(AvmObjectGetType(self));
        void* memory = AvmAlloc(size);
        AvmMemCopy((byte*)self, size, (byte*)memory, size);
        return memory;
    }

    return ((object(*)(object))method)(self);
}

AvmString AvmObjectToString(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction method = AvmObjectGetType(self)->_vptr[FnEntryToString];

    if (method == NULL) {
        return AvmSprintf("object <%x>", self);
    }

    return ((AvmString(*)(object))method)(self);
}
