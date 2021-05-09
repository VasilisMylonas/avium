/**
 * @file avium/typeinfo.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Type information for objects.
 * @version 0.1
 * @date 2021-05-09
 *
 * @copyright Copyright (c) 2021 Vasilis Mylonas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef AVIUM_TYPEINFO_H
#define AVIUM_TYPEINFO_H

#include "avium/types.h"

/// Represents an entry on the virtual function table.
typedef enum
{
    FnEntryDtor = 0,          ///< The destructor entry.
    FnEntryToString,          ///< The AvmObjectToString entry.
    FnEntryClone,             ///< The AvmObjectClone entry.
    FnEntryEquals,            ///< The AvmObjectEquals entry.
    FnEntryRead = 16,         ///< The AvmStreamRead entry.
    FnEntryWrite,             ///< The AvmStreamWrite entry.
    FnEntrySeek,              ///< The AvmStreamSeek entry.
    FnEntryFlush,             ///< The AvmStreamFlush entry.
    FnEntryGetPosition,       ///< The AvmStreamPosition entry.
    FnEntryGetBacktrace = 16, ///< The AvmErrorGetBacktrace entry.
    FnEntryGetSource,         ///< The AvmErrorGetSource entry.

    FnEntryGetLength = 12,
    FnEntryGetCapacity,
} AvmFnEntry;

/// Returns the base type of an object.
#define baseof(x) ((typeof((x)->_base)*)x)

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
    uint _size;
    const AvmType* _baseType;
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
AVMAPI str AvmTypeGetName(const AvmType* self);

/**
 * @brief Gets the size of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @return The type's size.
 */
AVMAPI uint AvmTypeGetSize(const AvmType* self);

/**
 * @brief Returns the specified VFT entry of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @param index The VFT entry.
 * @return The function pointer.
 */
AVMAPI AvmFunction AvmTypeGetFunction(const AvmType* self, uint index);

/**
 * @brief Returns a pointer to the type info for the base type of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @return The base type info.
 */
AVMAPI const AvmType* AvmTypeGetBase(const AvmType* self);

/**
 * @brief Determines whether a type inherits from another.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p baseType must be not null.
 *
 * @param self The AvmType instance.
 * @param baseType The base type info.
 * @return true if the type has the specified type as a base, otherwise false.
 */
AVMAPI bool AvmTypeInheritsFrom(const AvmType* self, const AvmType* baseType);

#ifndef DOXYGEN
static_assert_s(sizeof(AvmType) == AVM_TYPE_SIZE);
#endif

#endif // AVIUM_TYPEINFO_H
