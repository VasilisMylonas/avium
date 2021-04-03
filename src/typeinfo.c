#include "avium/typeinfo.h"
#include "avium/resources.h"

str AvmTypeGetName(const AvmType* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_name;
}

size_t AvmTypeGetSize(const AvmType* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_size;
}
