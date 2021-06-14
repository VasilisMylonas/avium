/**
 * @file avium/typeinfo.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Type information for language constructs.
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

#define baseof(x) (&(x)->__base)
#define typeid(T) (&_AVM_METADATA_BLOCK_NAME(T))
#define cast(T, x) ((T*)(__AvmRuntimeCast(x, typeid(T))))

// clang-format off

/// Determines whether an object is an instance of another type.
#define instanceof(T, x)                 \
    (typeid(T) == AvmObjectGetType(x) || \
     AvmClassInheritsFrom(AvmObjectGetType(x), typeid(T)))

// clang-format on

/**
 * @defgroup AvmCallMacros Macros for performing indirect function calls.
 *
 * @{
 */

/**
 * @brief Performs an interface function call.
 *
 * This macro expands to a function pointer.
 *
 * @param Entry The interface function entry.
 * @param TReturn The return type.
 * @param ... The parameter types.
 */
#define AVM_ICALL(Entry, TReturn, ...)                                         \
    ((TReturn(*)(__VA_ARGS__))self->Methods[Entry])

/**
 * @brief Performs a virtual function call.
 *
 * This macro expands to a function pointer.
 *
 * @param Entry The virtual function entry.
 * @param TReturn The return type.
 * @param ... The parameter types.
 */
#define AVM_VCALL(Entry, TReturn, ...)                                         \
    ((TReturn(*)(__VA_ARGS__))AvmClassGetCallback(                             \
        AvmObjectGetType((object)self), Entry))

/// @}

/**
 * @defgroup AvmTypeHandling Constructs for handling AvmType instances.
 *
 * @{
 */

/// Contains information about a type.
AVM_CLASS(AvmType, object, {
    struct
    {
        str name;
        uint size;
        union {
            uint constantCount;
            uint paramCount;
            uint vCount;
            uint offset;
        };
    } _private;
});

/**
 * @brief Returns the name of a type.
 *
 * @pre self != NULL.
 *
 * @param self The AvmType instance.
 * @return The type's name.
 */
AVMAPI str AvmTypeGetName(const AvmType* self) AVM_NONNULL(1);

/**
 * @brief Returns the size of a type.
 *
 * @pre self != NULL.
 *
 * @param self The AvmType instance.
 * @return The type's size.
 */
AVMAPI uint AvmTypeGetSize(const AvmType* self) AVM_NONNULL(1);

/// @}

/**
 * @defgroup AvmInterfaceHandling Constructs for handling AvmInterface
 *                                instances.
 *
 * @{
 */

/// Retrieves the interface I from an object.
#define AvmQueryInterface(self, I)                                             \
    (I)                                                                        \
    {                                                                          \
        .__type = typeid(I),                                                   \
        .Methods =                                                             \
            AvmClassGetInterface(AvmObjectGetType(self), #I)->_private.vPtr,   \
        .Value = self,                                                         \
    }

/// Contains information about an interface.
AVM_CLASS(AvmInterface, AvmType, {
    struct
    {
        const AvmCallback* vPtr;
    } _private;
});

/// A default interface initializer for the AVM_INTERFACES macro.
#define AVM_INTERFACES_DEFAULT                                                 \
    {                                                                          \
        [0] = NULL,                                                            \
    }

// TODO.
#define AVM_INTERFACES(T, ...)                                                 \
    static const AvmInterface* _AVM_INTERFACE_BLOCK_NAME(T)[] = __VA_ARGS__

// TODO.
#define interfaceid(I, T) (&_AVM_INTERFACE_IMPL_NAME(I, T))

/**
 * @brief Implements an interface for a type.
 *
 * @param I The interface to implement.
 * @param T The type to implement the interface for.
 * @param ... A braced enclosed array of function pointers (vtable).
 */
#define AVM_IMPLEMENT(I, T, ...)                                               \
    static const AvmCallback _AVM_INTERFACE_VTBL_NAME(I, T)[] = __VA_ARGS__;   \
    static const AvmInterface _AVM_INTERFACE_IMPL_NAME(I, T) = {               \
        .__base.__type = typeid(AvmInterface),                                 \
        .__base._private =                                                     \
            {                                                                  \
                .name = #I,                                                    \
                .size = sizeof(I),                                             \
                .vCount = sizeof(_AVM_INTERFACE_VTBL_NAME(I, T)) /             \
                          sizeof(AvmCallback),                                 \
            },                                                                 \
        ._private =                                                            \
            {                                                                  \
                .vPtr = _AVM_INTERFACE_VTBL_NAME(I, T),                        \
            },                                                                 \
    }

/// @}

/**
 * @defgroup AvmClassHandling Constructs for handling AvmClass instances.
 *
 * @{
 */

/// Contains information about a class.
AVM_CLASS(AvmClass, AvmType, {
    struct
    {
        const AvmClass* base;
        const AvmCallback* vPtr;
        const AvmInterface** interfaces;
        const AvmMember* members;
        uint interfaceCount;
        uint memberCount;
    } _private;
});

/// The default initializer for the vtable of the AVM_CLASS_TYPE(_EX) macro.
#define AVM_VTABLE_DEFAULT                                                     \
    {                                                                          \
        [0] = NULL,                                                            \
    }

/**
 * @brief Generates basic type information for a class type.
 *
 * @param T The class type.
 * @param B The base of the class type.
 * @param ... A brace enclosed array of function pointers (vtable).
 */
#define AVM_CLASS_TYPE(T, B, ...)                                              \
    static const AvmCallback _AVM_VTABLE_BLOCK_NAME(T)[] = __VA_ARGS__;        \
    const AvmClass _AVM_METADATA_BLOCK_NAME(T) = {                             \
        .__base.__type = typeid(AvmClass),                                     \
        .__base._private =                                                     \
            {                                                                  \
                .name = #T,                                                    \
                .size = sizeof(T),                                             \
                .vCount =                                                      \
                    sizeof(_AVM_VTABLE_BLOCK_NAME(T)) / sizeof(AvmCallback),   \
            },                                                                 \
        ._private =                                                            \
            {                                                                  \
                .base = typeid(B),                                             \
                .members = NULL,                                               \
                .interfaces = NULL,                                            \
                .vPtr = _AVM_VTABLE_BLOCK_NAME(T),                             \
                .memberCount = 0,                                              \
                .interfaceCount = 0,                                           \
            },                                                                 \
    }

/**
 * @brief Generates extended type information for a class type.
 *
 * The AVM_MEMBERS and AVM_INTERFACES macros must be expanded before this.
 *
 * @param T The class type.
 * @param B The base of the class type.
 * @param ... A brace enclosed array of function pointers (vtable).
 */
#define AVM_CLASS_TYPE_EX(T, B, ...)                                           \
    static const AvmCallback _AVM_VTABLE_BLOCK_NAME(T)[] = __VA_ARGS__;        \
    const AvmClass _AVM_METADATA_BLOCK_NAME(T) = {                             \
        .__base.__type = typeid(AvmClass),                                     \
        .__base._private =                                                     \
            {                                                                  \
                .name = #T,                                                    \
                .size = sizeof(T),                                             \
                .vCount =                                                      \
                    sizeof(_AVM_VTABLE_BLOCK_NAME(T)) / sizeof(AvmCallback),   \
            },                                                                 \
        ._private =                                                            \
            {                                                                  \
                .base = typeid(B),                                             \
                .members = _AVM_MEMBER_BLOCK_NAME(T),                          \
                .interfaces = _AVM_INTERFACE_BLOCK_NAME(T),                    \
                .vPtr = _AVM_VTABLE_BLOCK_NAME(T),                             \
                .memberCount =                                                 \
                    sizeof(_AVM_MEMBER_BLOCK_NAME(T)) / sizeof(AvmMember),     \
                .interfaceCount = sizeof(_AVM_INTERFACE_BLOCK_NAME(T)) /       \
                                  sizeof(AvmInterface*),                       \
            },                                                                 \
    }

// TODO: Undocumented. You probably want AvmQueryInterface.
AVMAPI const AvmInterface* AvmClassGetInterface(const AvmClass* self, str name)
    AVM_NONNULL(1, 2);

/**
 * @brief Determines whether a class implements an interface.
 *
 * @pre self != NULL.
 * @pre name != NULL.
 *
 * @param self The AvmClass instance.
 * @param name The interface name.
 * @return true if the class implements the interface, otherwise false.
 */
AVMAPI bool AvmClassImplements(const AvmClass* self, str name)
    AVM_NONNULL(1, 2);

// TODO: Undocumented. Prefer the AVM_VCALL macro.
AVMAPI AvmCallback AvmClassGetCallback(const AvmClass* self, uint index)
    AVM_NONNULL(1);

/**
 * @brief Returns the base of a class.
 *
 * @pre self != NULL.
 *
 * @param self The AvmClass instance.
 * @return The base class.
 */
AVMAPI const AvmClass* AvmClassGetBase(const AvmClass* self) AVM_NONNULL(1);

/**
 * @brief Determines whether a class inherits from another.
 *
 * @pre self != NULL.
 * @pre baseType != NULL.
 *
 * @param self The AvmClass instance.
 * @param baseType The base type.
 * @return true if the class inherits from the specified base, otherwise false.
 */
AVMAPI bool AvmClassInheritsFrom(const AvmClass* self, const AvmClass* baseType)
    AVM_NONNULL(1, 2);

/**
 * @brief Returns the class member at the specified index.
 *
 * @pre self != NULL.
 *
 * @param self The AvmClass instance.
 * @param index The member index.
 *
 * @return The class member.
 */
AVMAPI const AvmMember* AvmClassGetMemberAt(const AvmClass* self, uint index)
    AVM_NONNULL(1);

/**
 * @brief Returns the class member with the specified name.
 *
 * @pre self != NULL.
 * @pre name != NULL.
 *
 * @param self The AvmClass instance.
 * @param name The member name.
 *
 * @return The class member.
 */
AVMAPI const AvmMember* AvmClassGetMember(const AvmClass* self, str name)
    AVM_NONNULL(1, 2);

/**
 * @brief Returns the class member count.
 *
 * @pre self != NULL.
 *
 * @param self The AvmClass instance.
 * @return The member count.
 */
AVMAPI uint AvmClassGetMemberCount(const AvmClass* self) AVM_NONNULL(1);

/// @}

/**
 * @defgroup AvmMemberHandling Constructs for handling AvmMember instances.
 *
 * @{
 */

/// Contains information about a class member.
AVM_CLASS(AvmMember, AvmType, {
    struct
    {
        const AvmType* type;
    } _private;
});

/**
 * @brief Provides metadata for a single class member.
 *
 * @param P The parent class.
 * @param T The member type.
 * @param member The member name.
 */
#define AVM_MEMBER(P, T, member)                                               \
    {                                                                          \
        .__base.__type = typeid(AvmMember),                                    \
        .__base._private =                                                     \
            {                                                                  \
                .name = #member,                                               \
                .size = sizeof(T),                                             \
                .offset = offsetof(P, member),                                 \
            },                                                                 \
        ._private = {                                                          \
            .type = (AvmType*)typeid(T),                                       \
        },                                                                     \
    }

/// The default initializer for the member table of the AVM_MEMBERS macro.
#define AVM_MEMBERS_DEFAULT                                                    \
    {                                                                          \
        0                                                                      \
    }

/**
 * @brief Provides metadata about class members when used with AVM_CLASS_EX.
 *
 * @param T The type of the class.
 * @param ... A braced enclosed list of members (AVM_MEMBER).
 */
#define AVM_MEMBERS(T, ...)                                                    \
    static const AvmMember _AVM_MEMBER_BLOCK_NAME(T)[] = __VA_ARGS__

/**
 * @brief Returns the offset of the specified member.
 *
 * @pre self != NULL.
 *
 * @param self The AvmMember instance.
 * @return The member offset.
 */
AVMAPI uint AvmMemberGetOffset(const AvmMember* self) AVM_NONNULL(1);

/// @}

/**
 * @defgroup AvmEnumHandling Constructs for handling AvmEnum instances.
 *
 * @{
 */

/// Contains information about an enum.
AVM_CLASS(AvmEnum, AvmType, {
    struct
    {
        struct
        {
            str _name;
            _long _value;
        } * constants;
    } _private;
});

/**
 * @brief Generates type information for an enum.
 *
 * @param T The enum.
 * @param ... A braced enclosed array of enum constants (AVM_CONSTANT).
 */
#define AVM_ENUM_TYPE(T, ...)                                                  \
    static struct                                                              \
    {                                                                          \
        str _name;                                                             \
        _long _value;                                                          \
    } _AVM_PARAMS_BLOCK_NAME(T)[] = __VA_ARGS__;                               \
    const AvmEnum _AVM_METADATA_BLOCK_NAME(T) = {                              \
        .__base.__type = typeid(AvmEnum),                                      \
        .__base._private.name = #T,                                            \
        .__base._private.size = sizeof(T),                                     \
        .__base._private.base = typeid(object),                                \
        ._constants = (void*)_AVM_PARAMS_BLOCK_NAME(T),                        \
        __base._private._constantCount =                                       \
            sizeof(_AVM_PARAMS_BLOCK_NAME(T)) / (sizeof(_long) + sizeof(str)), \
    }

/**
 * @brief Provides metadata for an enum constant.
 *
 * @param C The enum constant.
 */
#define AVM_CONSTANT(C)                                                        \
    {                                                                          \
#C, C                                                                  \
    }

/**
 * @brief Determines whether a value is defined for an enum.
 *
 * @pre self != NULL.
 *
 * @param self The AvmEnum instance.
 * @param value The value.
 * @return true if a constant with the provided value is defined for the
 *         enum, otherwise false.
 */
AVMAPI bool AvmEnumIsDefined(const AvmEnum* self, _long value) AVM_NONNULL(1);

/**
 * @brief Returns the name of the enum constant with the specified value.
 *
 * @pre self != NULL.
 *
 * @param self The AvmEnum instance.
 * @param value The value of the constant.
 * @return The name of the constant.
 */
AVMAPI str AvmEnumGetNameOf(const AvmEnum* self, _long value) AVM_NONNULL(1);

/**
 * @brief Returns the value of the enum constant with the specified name.
 *
 * @pre self != NULL.
 * @pre name != NULL.
 *
 * @param self The AvmEnum instance.
 * @param name The name of the constant.
 * @return The value of the constant.
 */
AVMAPI _long AvmEnumGetValueOf(const AvmEnum* self, str name) AVM_NONNULL(1, 2);

/// @}

/**
 * @defgroup AvmFunctionHandling Constructs for handling AvmFunction instances.
 *
 * @{
 */

/// Contains information about a function.
AVM_CLASS(AvmFunction, AvmType, {
    struct
    {
        const AvmType** paramTypes;
        const AvmType* returnType;
        AvmCallback callback;
    } _private;
});

/**
 * @brief Generates type information for a function.
 *
 * @param F The function.
 * @param TReturn The return type.
 * @param ... The parameter type info (use typeid(void) for no parameters).
 */
#define AVM_FUNCTION_TYPE(F, TReturn, ...)                                     \
    static const AvmType* _AVM_PARAMS_BLOCK_NAME(F)[] = __VA_ARGS__;           \
    const AvmFunction _AVM_METADATA_BLOCK_NAME(F) = {                          \
        .__base.__type = typeid(AvmFunction),                                  \
        .__base._private =                                                     \
            {                                                                  \
                .name = #F,                                                    \
                .paramCount =                                                  \
                    sizeof(_AVM_METADATA_BLOCK_NAME(F)) / sizeof(AvmType*),    \
            },                                                                 \
        ._private = {                                                          \
            .paramTypes = _AVM_PARAMS_BLOCK_NAME(F),                           \
            .returnType = typeid(TReturn),                                     \
            .callback = (AvmCallback)(F),                                      \
        }}

/**
 * @brief Returns the return type of a function.
 *
 * @pre self != NULL.
 *
 * @param self The AvmFunction instance.
 * @return The return type.
 */
AVMAPI const AvmType* AvmFunctionGetReturnType(const AvmFunction* self)
    AVM_NONNULL(1);

/**
 * @brief Return the parameter type at the specified index.
 *
 * @pre self != NULL.
 * @pre index < AvmFunctionGetParamCount(self).
 *
 * @param self The AvmFunction instance.
 * @param index The index of the parameter.
 * @return The parameter type.
 */
AVMAPI const AvmType* AvmFunctionGetParam(const AvmFunction* self, uint index)
    AVM_NONNULL(1);

/**
 * @brief Returns the parameter count of a function.
 *
 * @pre self != NULL.
 *
 * @param self The AvmFunction instance.
 * @return The parameter count.
 */
AVMAPI uint AvmFunctionGetParamCount(const AvmFunction* self) AVM_NONNULL(1);

/// @}

#ifndef DOXYGEN
AVMAPI object __AvmRuntimeCast(object, const AvmClass*);

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
