#include "avium/collection.h"
#include "avium/resources.h"

size_t AvmCollectionGetLength(AvmCollection self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    const AvmType* type = AvmObjectGetType(self);
    AvmFunction method = type->_vptr[FUNC_GET_LENGTH];

    if (method == NULL) {
        AvmVirtualFunctionTrap();
    }

    return ((size_t(*)(AvmCollection))method)(self);
}

size_t AvmCollectionGetCapacity(AvmCollection self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    const AvmType* type = AvmObjectGetType(self);
    AvmFunction method = type->_vptr[FUNC_GET_CAPACITY];

    if (method == NULL) {
        AvmVirtualFunctionTrap();
    }

    return ((size_t(*)(AvmCollection))method)(self);
}
