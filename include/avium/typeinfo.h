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

#include "avium/core.h"

/**
 * @brief Generates type info for a type.
 *
 * @param T The type for which to generate type info.
 * @param B The base type.
 * @param ... The type vtable enclosed in braces ({...})
 */
#define AVM_TYPE(T, B, ...)                                                    \
    static AvmFunction AVM_VT_NAME(T)[] = __VA_ARGS__;                         \
    const AvmType AVM_TI_NAME(T) = {                                           \
        ._type = typeid(AvmType),                                              \
        ._vPtr = AVM_VT_NAME(T),                                               \
        ._name = #T,                                                           \
        ._baseType = typeid(B),                                                \
        ._vSize = sizeof(AVM_VT_NAME(T)),                                      \
        ._size = sizeof(T),                                                    \
    }

/// A type containing information about an object.
AVM_CLASS(AvmType, object, {
    const AvmType* _baseType;
    str _name;
    uint _size;
    uint _vSize;
    AvmFunction* _vPtr;
});

#define AVM_ENUM_MEMBER(V)                                                     \
    {                                                                          \
#V, V                                                                  \
    }

/**
 * @brief Generates type info for an enum.
 *
 * @param T The enum for which to generate type info.
 * @param ... The enum members enclosed in braces ({...})
 */
#define AVM_ENUM_TYPE(T, ...)                                                  \
    const AvmEnum AVM_TI_NAME(T) = {                                           \
        ._type = typeid(AvmEnum),                                              \
        ._name = #T,                                                           \
        ._size = sizeof(T),                                                    \
        ._members = __VA_ARGS__,                                               \
    }

/// A type containing information about an enum.
AVM_CLASS(AvmEnum, object, {
    str _name;
    uint _size;
    struct
    {
        str _name;
        _long _value;
    } _members[AVM_MAX_ENUM_MEMBERS];
});

/// Represents an entry on the virtual function table.
AVM_ENUM(AvmFnEntry,
         {
             FnEntryDtor = 0,    ///< The destructor entry.
             FnEntryToString,    ///< The AvmObjectToString entry.
             FnEntryClone,       ///< The AvmObjectClone entry.
             FnEntryEquals,      ///< The AvmObjectEquals entry.
             FnEntryRead = 16,   ///< The AvmStreamRead entry.
             FnEntryWrite,       ///< The AvmStreamWrite entry.
             FnEntrySeek,        ///< The AvmStreamSeek entry.
             FnEntryFlush,       ///< The AvmStreamFlush entry.
             FnEntryGetPosition, ///< The AvmStreamPosition entry.

             FnEntryGetLength = 12,
             FnEntryGetCapacity,

             FnEntryRemove = 16,
             FnEntryInsert,
             FnEntryItemAt,
             FnEntryGetItemType,
         });

/// Returns the base type of an object.
#define baseof(x) (&(x)->_base)

/// Returns a pointer to the type info of type T.
#define typeid(T) (&AVM_TI_NAME(T))

/// Refers to the base type in a function with a self parameter.
#define base (&self->_base)

// clang-format off

/// Determines whether an object is an instance of another type.
#define instanceof(T, x)                 \
    (typeid(T) == AvmObjectGetType(x) || \
     AvmTypeInheritsFrom(AvmObjectGetType(x), typeid(T)))

#ifdef AVM_EXIT_ON_CAST_FAIL
#define cast(T, x) (T*)(instanceof(T, x) ? x : __AvmRuntimeCastFail(x, typeid(T)))
#else
#define cast(T, x) (T*)(instanceof(T, x) ? x : NULL)
#endif
// clang-format on

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

/**
 * @brief Constructs an object from an AvmType instance.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @return The constructed object.
 */
AVMAPI object AvmTypeConstruct(const AvmType* self);

/**
 * @brief Returns the name of an enum.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmEnum instance.
 * @return The enum name.
 */
AVMAPI str AvmEnumGetName(const AvmEnum* self);

/**
 * @brief Returns the size of an enum.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmEnum instance.
 * @return The enum size.
 */
AVMAPI uint AvmEnumGetSize(const AvmEnum* self);

/**
 * @brief Determines whether a value is defined for an enum.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmEnum instance.
 * @param value The value.
 * @return true if a constant with the provided value is defined for the enum,
 *         otherwise false.
 */
AVMAPI bool AvmEnumIsDefined(const AvmEnum* self, _long value);

/**
 * @brief Returns the name of the enum constant with the specified value.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmEnum instance.
 * @param value The value of the constant.
 * @return The name of the constant.
 */
AVMAPI str AvmEnumGetNameOf(const AvmEnum* self, _long value);

/**
 * @brief Returns the value of the enum constant with the specified name.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p name must be not null.
 *
 * @param self The AvmEnum instance.
 * @param name The name of the constant.
 * @return The value of the constant.
 */
AVMAPI _long AvmEnumGetValueOf(const AvmEnum* self, str name);

AVMAPI object __AvmRuntimeCastFail(object, const AvmType*);

#ifndef DOXYGEN
#define DOXYGEN
extern const AvmType AVM_TI_NAME(_long);
extern const AvmType AVM_TI_NAME(ulong);
extern const AvmType AVM_TI_NAME(int);
extern const AvmType AVM_TI_NAME(uint);
extern const AvmType AVM_TI_NAME(short);
extern const AvmType AVM_TI_NAME(ushort);
extern const AvmType AVM_TI_NAME(char);
extern const AvmType AVM_TI_NAME(byte);
extern const AvmType AVM_TI_NAME(object);
extern const AvmType AVM_TI_NAME(float);
extern const AvmType AVM_TI_NAME(double);
extern const AvmType AVM_TI_NAME(str);
#endif // DOXYGEN

#endif // AVIUM_TYPEINFO_H
