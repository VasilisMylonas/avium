#include "avium/collection.h"
#include "avium/internal.h"

size_t AvmCollectionGetLength(AvmCollection self) {
    const AvmType* type = AvmObjectGetType(self);
    AvmFunction method = type->_vptr[FUNC_GET_LENGTH];

    if (method == NULL) {
        AvmVirtualFunctionTrap(__func__, type);
    }

    return ((size_t(*)(AvmCollection))method)(self);
}

size_t AvmCollectionGetCapacity(AvmCollection self) {
    const AvmType* type = AvmObjectGetType(self);
    AvmFunction method = type->_vptr[FUNC_GET_CAPACITY];

    if (method == NULL) {
        AvmVirtualFunctionTrap(__func__, type);
    }

    return ((size_t(*)(AvmCollection))method)(self);
}
