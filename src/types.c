#include <string.h>  // For memcmp, memcpy
#include <stdlib.h>  // For malloc

#include "avium/types.h"
#include "avium/runtime.h"
#include "avium/resources.h"
#include "avium/fmt.h"

const AvmType* AvmObjectGetType(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

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
    AvmFunction method = type->_vptr[FUNC_EQUALS];
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

    AvmFunction method = AvmObjectGetType(self)->_vptr[FUNC_DTOR];

    if (method != NULL) {
        ((void (*)(object))method)(self);
    }
}

object AvmObjectClone(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction method = AvmObjectGetType(self)->_vptr[FUNC_CLONE];

    if (method == NULL) {
        size_t size = AvmTypeGetSize(AvmObjectGetType(self));
        return memcpy(malloc(size), self, size);
    }

    return ((object(*)(object))method)(self);
}

AvmString AvmObjectToString(object self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction method = AvmObjectGetType(self)->_vptr[FUNC_TO_STRING];

    if (method == NULL) {
        return AvmSprintf("object <%x>", self);
    }

    return ((AvmString(*)(object))method)(self);
}
