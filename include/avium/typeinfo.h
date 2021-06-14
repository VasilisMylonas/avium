/**
 * @file avium/typeinfo.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Type information for objects.
 * @version 0.3
 * @date 2021-06-14
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

/// Contains information about a type.
AVM_CLASS(AvmType, object, {
    struct
    {
        str name;
        uint size;
        uint unused;
    } _private;
});

/// Contains information about a function.
AVM_CLASS(AvmFunction, AvmType, {
    struct
    {
        uint paramCount;
        const AvmType** paramTypes;
        const AvmType* returnType;
        AvmCallback callback;
    } _private;
});

/// Contains information about an interface.
AVM_CLASS(AvmInterface, AvmType, {
    struct
    {
        const AvmCallback* vPtr;
        uint vCount;
        uint unused;
    } _private;
});

/// Contains information about a class.
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

/// Contains information about an enum.
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

/// Contains information about a class member.
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
        .__type = typeid(AvmMember),                                           \
        .__base._private =                                                     \
            {                                                                  \
                .name = #member,                                               \
                .size = sizeof(T),                                             \
            },                                                                 \
        ._private = {                                                          \
            .offset = offsetof(P, member),                                     \
            .type = (AvmType*)typeid(T),                                       \
        },                                                                     \
    }

#define AVM_IMPLEMENT(I, T, ...)                                               \
    static const AvmCallback _AVM_INTERFACE_VTBL_NAME(I, T)[] = __VA_ARGS__;   \
    static const AvmInterface _AVM_INTERFACE_IMPL_NAME(I, T) = {               \
        .__type = typeid(AvmInterface),                                        \
        .__base._private =                                                     \
            {                                                                  \
                .name = #I,                                                    \
                .size = sizeof(I),                                             \
            },                                                                 \
        ._private =                                                            \
            {                                                                  \
                .vPtr = _AVM_INTERFACE_VTBL_NAME(I, T),                        \
                .vCount = sizeof(_AVM_INTERFACE_VTBL_NAME(I, T)),              \
            },                                                                 \
    }

#define AVM_CLASS_TYPE(T, B, ...)                                              \
    static const AvmCallback _AVM_VTABLE_BLOCK_NAME(T)[] = __VA_ARGS__;        \
    const AvmClass _AVM_METADATA_BLOCK_NAME(T) = {                             \
        .__type = typeid(AvmClass),                                            \
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
    static const AvmInterface* _AVM_INTERFACE_BLOCK_NAME(T)[] = __VA_ARGS__

#define AVM_CLASS_TYPE_EX(T, B, ...)                                           \
    static const AvmCallback _AVM_VTABLE_BLOCK_NAME(T)[] = __VA_ARGS__;        \
    const AvmClass _AVM_METADATA_BLOCK_NAME(T) = {                             \
        .__type = typeid(AvmClass),                                            \
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
    } _AVM_PARAMS_BLOCK_NAME(T)[] = __VA_ARGS__;                               \
    const AvmEnum _AVM_METADATA_BLOCK_NAME(T) = {                              \
        .__type = typeid(AvmEnum),                                             \
        .__base._private.name = #T,                                            \
        .__base._private.size = sizeof(T),                                     \
        .__base._private.base = typeid(object),                                \
        ._constants = (void*)_AVM_PARAMS_BLOCK_NAME(T),                        \
        ._constantCount =                                                      \
            sizeof(_AVM_PARAMS_BLOCK_NAME(T)) / (sizeof(_long) + sizeof(str)), \
    }

#define AVM_FUNCTION_TYPE(F, TReturn, ...)                                     \
    static const AvmType* _AVM_PARAMS_BLOCK_NAME(F)[] = __VA_ARGS__;           \
    const AvmFunction _AVM_METADATA_BLOCK_NAME(F) = {                          \
        .__type = typeid(AvmFunction),                                         \
        .__base._private.name = #F,                                            \
        ._private = {                                                          \
            .paramCount =                                                      \
                sizeof(_AVM_METADATA_BLOCK_NAME(F)) / sizeof(AvmType*),        \
            .paramTypes = _AVM_PARAMS_BLOCK_NAME(F),                           \
            .returnType = typeid(TReturn),                                     \
            .callback = (AvmCallback)(F),                                      \
        }}

#define AVM_ICALL(Entry, TReturn, ...)                                         \
    ((TReturn(*)(__VA_ARGS__))self->Methods[Entry])

#define AVM_VCALL(Entry, TReturn, ...)                                         \
    ((TReturn(*)(__VA_ARGS__))AvmClassGetCallback(                             \
        AvmObjectGetType((object)self), Entry))

#define AvmQueryInterface(self, I)                                             \
    (I)                                                                        \
    {                                                                          \
        .Methods =                                                             \
            AvmClassGetInterface(AvmObjectGetType(self), #I)->_private.vPtr,   \
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

AVMAPI const AvmInterface* AvmClassGetInterface(const AvmClass* self, str name);
AVMAPI AvmCallback AvmClassGetCallback(const AvmClass* self, uint index);
AVMAPI const AvmClass* AvmClassGetBase(const AvmClass* self);
AVMAPI
bool AvmClassInheritsFrom(const AvmClass* self, const AvmClass* baseType);

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
AVMAPI uint AvmMemberGetOffset(const AvmMember* self);

/**
 * @brief Determines whether a value is defined for an enum.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmEnum instance.
 * @param value The value.
 * @return true if a constant with the provided value is defined for the
 * enum, otherwise false.
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

#ifndef DOXYGEN
AVMAPI object __AvmRuntimeCastFail(object, const AvmClass*);

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
