#ifndef AVIUM_TYPEINFO_H
#define AVIUM_TYPEINFO_H

#include "avium/types.h"

#define AVM_GET_TYPE(T)  AVM_GET_TYPE_(T)
#define AVM_TYPE(T, ...) AVM_TYPE_(T, __VA_ARGS__)

struct AvmType {
    const AvmFunction* _vptr;
    str _name;
    size_t _size;
};

/**
 * @brief Gets the name of a type.
 *
 * @param self The type.
 * @return str The type's name.
 */
AVMAPI str AvmTypeGetName(const AvmType* self);

/**
 * @brief Gets the size of a type.
 *
 * @param self The type.
 * @return size_t The type's size.
 */
AVMAPI size_t AvmTypeGetSize(const AvmType* self);

#define AVM_TYPE_(T, ...)                                \
    static const AvmType _##T##Type = {                  \
        ._vptr = (AvmFunction[AVM_VFT_SIZE])__VA_ARGS__, \
        ._name = #T,                                     \
        ._size = sizeof(T),                              \
    }

#define AVM_GET_TYPE_(T) &_##T##Type

#endif  // AVIUM_TYPEINFO_H
