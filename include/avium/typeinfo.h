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
        .__base.__type = typeid(AvmEnum),                                      \
        .__base._name = #T,                                                    \
        .__base._size = sizeof(T),                                             \
        .__base.__baseType = typeid(object),                                   \
        ._cPtr = (void*)AVM_CTI_NAME(T),                                       \
        ._cSize = sizeof(AVM_CTI_NAME(T)),                                     \
    }

#define AVM_FUNCTION_TYPE(F, TReturn, ...)                                     \
    static const AvmType* AVM_PTI_NAME(F)[] = __VA_ARGS__;                     \
    const AvmFunction AVM_TI_NAME(F) = {                                       \
        .__type = typeid(AvmFunction),                                         \
        ._paramCount = sizeof(AVM_PTI_NAME(F)) / sizeof(AvmType*),             \
        ._paramTypes = AVM_PTI_NAME(F),                                        \
        ._returnType = typeid(TReturn),                                        \
        ._callback = (AvmCallback)(F),                                         \
        ._name = #F,                                                           \
    }

AVM_CLASS(AvmType, object, {
    struct
    {
        str name;
        const byte* init;
        uint size;
        uint unused;
    } _private;
});

// TODO
AVM_CLASS(AvmFunction, AvmType, {
    uint _paramCount;
    const AvmType** _paramTypes;
    const AvmType* _returnType;
    AvmCallback _callback;
    str _name;
});

AVM_CLASS(AvmInterface, AvmType, {
    struct
    {
        const AvmCallback* vPtr;
        uint vCount;
        uint unused;
    } _private;
});

AVM_CLASS(AvmClass, AvmType, {
    struct
    {
        const AvmClass* base;
        const AvmCallback* vPtr;
        const AvmInterface** interfaces;
        const AvmMember* members;
        ushort vCount;
        ushort interfaceCount;
        ushort memberCount;
        ushort unused;
    } _private;
});

AVM_CLASS(AvmEnum, AvmType, {
    struct
    {
        struct
        {
            str _name;
            _long _value;
        } * constants;
        ushort constantCount;
        ushort unused0;
        uint unused1;
    } _private;
});

AVM_CLASS(AvmMember, AvmType, {
    struct
    {
        uint offset;
        uint unused;
        const AvmType* type;
    } _private;
});

#define AVM_MEMBER(P, T, member)                                               \
    {                                                                          \
        .__type = typeid(AvmMember), ._name = #member,                         \
        ._offset = offsetof(P, member), ._memberType = typeid(T),              \
    }

#define AVM_INTERFACE(T, ...)                                                  \
    enum __VA_ARGS__;                                                          \
    AVM_CLASS(T, object, {                                                     \
        object Value;                                                          \
        const AvmCallback* Methods;                                            \
    })

#define AVM_IMPLEMENT(I, T, ...)                                               \
    static const AvmCallback _AVM_INTERFACE_VTBL_NAME(I, T)[] = __VA_ARGS__;   \
    static const AvmInterface _AVM_INTERFACE_IMPL_NAME(I, T) = {               \
        .__type = typeid(AvmInterface),                                        \
        ._private =                                                            \
            {                                                                  \
                .vPtr = _AVM_INTERFACE_VTBL_NAME(I, T),                        \
                .vCount = sizeof(_AVM_INTERFACE_VTBL_NAME(I, T)),              \
            },                                                                 \
        .__base._private =                                                     \
            {                                                                  \
                .name = #I,                                                    \
            },                                                                 \
    }

#define AVM_CLASS_TYPE(T, B, ...)                                              \
    static const AvmCallback _AVM_VTABLE_BLOCK_NAME(T)[] = __VA_ARGS__;        \
    const AvmClass _AVM_METADATA_BLOCK_NAME(T) = {                             \
        .__base._private =                                                     \
            {                                                                  \
                .name = #T,                                                    \
                .size = sizeof(T),                                             \
            },                                                                 \
        ._private =                                                            \
            {                                                                  \
                .base = typeid(B),                                             \
                .vPtr = _AVM_VTABLE_BLOCK_NAME(T),                             \
                .vCount =                                                      \
                    sizeof(_AVM_VTABLE_BLOCK_NAME(T)) / sizeof(AvmCallback),   \
            },                                                                 \
    }

#define AVM_MEMBERS(T, ...)                                                    \
    static const AvmMember _AVM_MEMBER_BLOCK_NAME(T)[] = __VA_ARGS__

#define AVM_INTERFACES(T, ...)                                                 \
    static const AvmInterface* _AVM_INTERFACE_BLOCK_NAME(T)[] = __VA_ARGS__;

#define AVM_CLASS_TYPE_EX(T, B, ...)                                           \
    static const AvmCallback _AVM_VTABLE_BLOCK_NAME(T)[] = __VA_ARGS__;        \
    const AvmClass _AVM_METADATA_BLOCK_NAME(T) = {                             \
        .__base._private =                                                     \
            {                                                                  \
                .name = #T,                                                    \
                .size = sizeof(T),                                             \
            },                                                                 \
        ._private =                                                            \
            {                                                                  \
                .base = typeid(B),                                             \
                .members = _AVM_MEMBER_BLOCK_NAME(T),                          \
                .interfaces = _AVM_INTERFACE_BLOCK_NAME(T),                    \
                .vPtr = _AVM_VTABLE_BLOCK_NAME(T),                             \
                .vCount =                                                      \
                    sizeof(_AVM_VTABLE_BLOCK_NAME(T)) / sizeof(AvmCallback),   \
                .memberCount =                                                 \
                    sizeof(_AVM_MEMBER_BLOCK_NAME(T)) / sizeof(AvmMember),     \
                .interfaceCount = sizeof(_AVM_INTERFACE_BLOCK_NAME(T)) /       \
                                  sizeof(AvmInterface*),                       \
            },                                                                 \
    }

AVMAPI const AvmInterface* AvmClassGetInterface(const AvmClass* self, str name);

#define icall(Entry, TReturn, ...)                                             \
    ((TReturn(*)(__VA_ARGS__))self->Methods[Entry])

#define QueryInterface(self, I)                                                \
    (I)                                                                        \
    {                                                                          \
        .Methods =                                                             \
            AvmClassGetInterface((const AvmClass*)AvmObjectGetType(self), #I)  \
                ->_private.vPtr,                                               \
        .Value = self,                                                         \
    }

#define AVM_VTABLE_DEFAULT                                                     \
    {                                                                          \
        [0] = NULL,                                                            \
    }

#define AVM_MEMBERS_DEFAULT                                                    \
    {                                                                          \
        0                                                                      \
    }

#define AVM_INTERFACES_DEFAULT                                                 \
    {                                                                          \
        [0] = NULL,                                                            \
    }

enum
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
};

/// Returns the base type of an object.
#define baseof(x) (&(x)->__base)

/// Returns a pointer to the type info of type T.
#define typeid(T) (&_AVM_METADATA_BLOCK_NAME(T))

#define interfaceof(I, T) (&_AVM_INTERFACE_IMPL_NAME(I, T))

// clang-format off

/// Determines whether an object is an instance of another type.
#define instanceof(T, x)                 \
    (typeid(T) == AvmObjectGetType(x) || \
     AvmClassInheritsFrom(AvmObjectGetType(x), typeid(T)))
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

AVMAPI const byte* AvmTypeGetInit(const AvmType* self);

AVMAPI AvmCallback AvmClassGetCallback(const AvmClass* self, uint index);
AVMAPI const AvmClass* AvmClassGetBase(const AvmClass* self);
AVMAPI bool AvmClassInheritsFrom(const AvmClass* self,
                                 const AvmClass* baseType);

AVMAPI uint AvmClassGetMemberCount(const AvmClass* self);
AVMAPI const AvmMember* AvmClassGetMemberAt(const AvmClass* self, uint index);
AVMAPI const AvmMember* AvmClassGetMember(const AvmClass* self, str name);

/**
 * @brief Returns the offset of the specified member.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmMember instance.
 * @return The member offset.
 */
// AVMAPI uint AvmMemberGetOffset(const AvmMember* self);

/**
 * @brief Returns the name of the specified member.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmMember instance.
 * @return The member name.
 */
// AVMAPI str AvmMemberGetName(const AvmMember* self);

/**
 * @brief Returns the name of an enum.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmEnum instance.
 * @return The enum name.
 */
// AVMAPI str AvmEnumGetName(const AvmEnum* self);

/**
 * @brief Returns the size of an enum.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmEnum instance.
 * @return The enum size.
 */
// AVMAPI uint AvmEnumGetSize(const AvmEnum* self);

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
// AVMAPI bool AvmEnumIsDefined(const AvmEnum* self, _long value);

/**
 * @brief Returns the name of the enum constant with the specified value.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmEnum instance.
 * @param value The value of the constant.
 * @return The name of the constant.
 */
// AVMAPI str AvmEnumGetNameOf(const AvmEnum* self, _long value);

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
// AVMAPI _long AvmEnumGetValueOf(const AvmEnum* self, str name);

AVMAPI const AvmType* AvmFunctionGetReturnType(const AvmFunction* self);
AVMAPI const AvmType** AvmFunctionGetParams(const AvmFunction* self);
AVMAPI uint AvmFunctionGetParamCount(const AvmFunction* self);
AVMAPI str AvmFunctionGetName(const AvmFunction* self);

#ifndef DOXYGEN
AVMAPI object __AvmRuntimeCastFail(object, const AvmType*);

AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(_long);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(ulong);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(int);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(uint);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(short);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(ushort);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(char);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(byte);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(bool);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(float);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(double);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(str);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(object);
AVMAPI const AvmClass _AVM_METADATA_BLOCK_NAME(void);
#endif // DOXYGEN

#endif // AVIUM_TYPEINFO_H
