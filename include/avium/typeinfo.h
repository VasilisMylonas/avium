#ifndef AVIUM_TYPEINFO_H
#define AVIUM_TYPEINFO_H

#include "avium/types.h"

/// Returns a pointer to the type info of type T.
#define typeid(T) (&AVM_TI_NAME(T))

// clang-format off

/// Determines whether an object is an instance of another type.
#define instanceof(T, x)                 \
    (typeid(T) == AvmObjectGetType(x) || \
     AvmTypeInheritsFrom(AvmObjectGetType(x), typeid(T)))

// clang-format on

/// A type containing information about an object.
AVM_CLASS(AvmType, object, {
    str _name;
    size_t _size;
    const AvmType *_baseType;
    AvmFunction _vptr[AVM_VFT_SIZE];
});

/**
 * @brief Gets the name of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @return The type's name.
 */
AVMAPI str AvmTypeGetName(const AvmType *self);

/**
 * @brief Gets the size of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @return The type's size.
 */
AVMAPI size_t AvmTypeGetSize(const AvmType *self);

/**
 * @brief Returns the specified VFT entry of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @param index The VFT entry.
 * @return The function pointer.
 */
AVMAPI AvmFunction AvmTypeGetFunction(const AvmType *self, size_t index);

AVMAPI const AvmType *AvmTypeGetBase(const AvmType *self);

AVMAPI bool AvmTypeInheritsFrom(const AvmType *self, const AvmType *baseType);

#endif // AVIUM_TYPEINFO_H
