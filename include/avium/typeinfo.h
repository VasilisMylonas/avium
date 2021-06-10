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
    static AvmCallback AVM_VT_NAME(T)[] = __VA_ARGS__;                         \
    const AvmType AVM_TI_NAME(T) = {                                           \
        ._type = typeid(AvmType),                                              \
        ._vPtr = AVM_VT_NAME(T),                                               \
        ._name = #T,                                                           \
        ._baseType = typeid(B),                                                \
        ._vSize = sizeof(AVM_VT_NAME(T)),                                      \
        ._size = sizeof(T),                                                    \
    }

#define AVM_TYPE_EX(T, B, ...)                                                 \
    static AvmCallback AVM_VT_NAME(T)[] = __VA_ARGS__;                         \
    const AvmType AVM_TI_NAME(T) = {                                           \
        ._type = typeid(AvmType),                                              \
        ._vPtr = AVM_VT_NAME(T),                                               \
        ._name = #T,                                                           \
        ._baseType = typeid(B),                                                \
        ._vSize = sizeof(AVM_VT_NAME(T)),                                      \
        ._size = sizeof(T),                                                    \
        ._mSize = sizeof(AVM_MTI_NAME(T)),                                     \
        ._mPtr = AVM_MTI_NAME(T),                                              \
    }

/// A type containing information about an object.
AVM_CLASS(AvmType, object, {
    const AvmType* _baseType;
    str _name;
    uint _size;
    ushort _vSize;
    ushort _mSize;
    AvmCallback* _vPtr;
    const AvmMember* _mPtr;
});

#define AVM_MEMBER(P, T, member)                                               \
    {                                                                          \
        ._type = typeid(AvmMember), ._name = #member,                          \
        ._offset = offsetof(P, member), ._memberType = typeid(T),              \
    }

#define AVM_MEMBERS(T, ...)                                                    \
    static const AvmMember AVM_MTI_NAME(T)[] = __VA_ARGS__

/// Represents a struct member.
AVM_CLASS(AvmMember, object, {
    const AvmType* _memberType;
    str _name;
    uint _offset;
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
    static struct                                                              \
    {                                                                          \
        str _name;                                                             \
        _long _value;                                                          \
    } AVM_CTI_NAME(T)[] = __VA_ARGS__;                                         \
    const AvmEnum AVM_TI_NAME(T) = {                                           \
        ._base._type = typeid(AvmEnum),                                        \
        ._base._name = #T,                                                     \
        ._base._size = sizeof(T),                                              \
        ._base._baseType = typeid(object),                                     \
        ._cPtr = (void*)AVM_CTI_NAME(T),                                       \
        ._cSize = sizeof(AVM_CTI_NAME(T)),                                     \
    }

/// A type containing information about an enum.
AVM_CLASS(AvmEnum, AvmType, {
    uint _cSize;
    struct
    {
        str _name;
        _long _value;
    } * _cPtr;
});

#define AVM_FUNCTION_TYPE(F, TReturn, ...)                                     \
    static const AvmType* AVM_PTI_NAME(F)[] = __VA_ARGS__;                     \
    const AvmFunction AVM_TI_NAME(F) = {                                       \
        ._type = typeid(AvmFunction),                                          \
        ._paramCount = sizeof(AVM_PTI_NAME(F)) / sizeof(AvmType*),             \
        ._paramTypes = AVM_PTI_NAME(F),                                        \
        ._returnType = typeid(TReturn),                                        \
        ._callback = (AvmCallback)(F),                                         \
        ._name = #F,                                                           \
    }

AVM_CLASS(AvmFunction, object, {
    uint _paramCount;
    const AvmType** _paramTypes;
    const AvmType* _returnType;
    AvmCallback _callback;
    str _name;
});

/// Represents an entry on the virtual function table.
AVM_ENUM(AvmFnEntry,
         {
             FnEntryFinalize = 0, ///< The finalizer entry.
             FnEntryToString,     ///< The AvmObjectToString entry.
             FnEntryClone,        ///< The AvmObjectClone entry.
             FnEntryEquals,       ///< The AvmObjectEquals entry.

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

// clang-format off

/// Determines whether an object is an instance of another type.
#define instanceof(T, x)                 \
    (typeid(T) == AvmObjectGetType(x) || \
     AvmTypeInheritsFrom(AvmObjectGetType(x), typeid(T)))
// clang-format on

#define cast(T, x)                                                             \
    (T*)(instanceof (T, x) ? x : __AvmRuntimeCastFail(x, typeid(T)))

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
 * @return The callback.
 */
AVMAPI AvmCallback AvmTypeGetCallback(const AvmType* self, uint index);

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
 * @brief Returns the number of members of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @return The number of members of the type.
 */
AVMAPI uint AvmTypeGetMemberCount(const AvmType* self);

/**
 * @brief Returns the member at the specified index.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @param index The index of the member.
 * @return The AvmMember instance.
 */
AVMAPI const AvmMember* AvmTypeGetMemberAt(const AvmType* self, uint index);

/**
 * @brief Returns the specified member.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p name must be not null.
 *
 * @param self The AvmType instance.
 * @param name The name of the member.
 * @return The AvmMember instance.
 */
AVMAPI const AvmMember* AvmTypeGetMember(const AvmType* self, str name);

/**
 * @brief Returns the offset of the specified member.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmMember instance.
 * @return The member offset.
 */
AVMAPI uint AvmMemberGetOffset(const AvmMember* self);

/**
 * @brief Returns the name of the specified member.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmMember instance.
 * @return The member name.
 */
AVMAPI str AvmMemberGetName(const AvmMember* self);

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

AVMAPI const AvmType* AvmFunctionGetReturnType(const AvmFunction* self);
AVMAPI const AvmType** AvmFunctionGetParams(const AvmFunction* self);
AVMAPI uint AvmFunctionGetParamCount(const AvmFunction* self);
AVMAPI str AvmFunctionGetName(const AvmFunction* self);

#define AVM_VFT_DEFAULT                                                        \
    {                                                                          \
        [FnEntryFinalize] = NULL                                               \
    }

#ifndef DOXYGEN
AVMAPI object __AvmRuntimeCastFail(object, const AvmType*);

AVMAPI const AvmType AVM_TI_NAME(_long);
AVMAPI const AvmType AVM_TI_NAME(ulong);
AVMAPI const AvmType AVM_TI_NAME(int);
AVMAPI const AvmType AVM_TI_NAME(uint);
AVMAPI const AvmType AVM_TI_NAME(short);
AVMAPI const AvmType AVM_TI_NAME(ushort);
AVMAPI const AvmType AVM_TI_NAME(char);
AVMAPI const AvmType AVM_TI_NAME(byte);
AVMAPI const AvmType AVM_TI_NAME(bool);
AVMAPI const AvmType AVM_TI_NAME(float);
AVMAPI const AvmType AVM_TI_NAME(double);
AVMAPI const AvmType AVM_TI_NAME(str);
AVMAPI const AvmType AVM_TI_NAME(object);
AVMAPI const AvmType AVM_TI_NAME(void);
#endif // DOXYGEN

#endif // AVIUM_TYPEINFO_H
