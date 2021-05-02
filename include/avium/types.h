/**
 * @file avium/types.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Primitive Avium types and related functions.
 * @version 0.2.2
 * @date 2021-05-01
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

#ifndef AVIUM_TYPES_H
#define AVIUM_TYPES_H

#include "avium/config.h"
#include "avium/exports.h"

#include <stdarg.h>   // For va_list
#include <stdbool.h>  // For bool, true, false
#include <stddef.h>   // For NULL, size_t

/**
 * @brief Creates an Avium class type.
 *
 * @param T The name of the type.
 * @param B The base class of the type.
 * @param ... Member declaration in braces ({ ... })
 */
#define AVM_CLASS(T, B, ...)             \
    extern const AvmType AVM_TI_NAME(T); \
    typedef struct T T;                  \
    struct T {                           \
        union {                          \
            const AvmType* _type;        \
            B _base;                     \
        };                               \
        struct __VA_ARGS__;              \
    }

/**
 * @brief Creates an Avium inline class type.
 *
 * Use this for generic or otherwise header-only classes. It must be used with
 * the AVM_INLINE_TYPE macro.
 *
 * @param T The name of the type.
 * @param B The base class of the type.
 * @param ... Member declaration in braces ({ ... })
 */
#define AVM_INLINE_CLASS(T, B, ...) \
    typedef struct T T;             \
    struct T {                      \
        union {                     \
            const AvmType* _type;   \
            B _base;                \
        };                          \
        struct __VA_ARGS__;         \
    }

/**
 * @brief Creates an Avium interface type.
 *
 * @param T The name of the type.
 */
#define AVM_INTERFACE(T)                 \
    extern const AvmType AVM_TI_NAME(T); \
    typedef struct T T;                  \
    struct T {                           \
        const AvmType* _type;            \
    }

/**
 * @brief Generates type info for a type.
 *
 * @param T The type for which to generate type info.
 * @param B The base type.
 * @param ... The type vtable enclosed in braces ({...})
 */
#define AVM_TYPE(T, B, ...) AVM_TYPE_(T, B, __VA_ARGS__)

/**
 * @brief Generates inline type info for a type.
 *
 * Used with the AVM_INLINE_CLASS macro.
 *
 * @param T The type for which to generate type info.
 * @param B The base type.
 * @param ... The type vtable enclosed in braces ({...})
 */
#define AVM_INLINE_TYPE(T, B, ...) static AVM_TYPE_(T, B, __VA_ARGS__)

/// Convieniece macro for type-generic types.
#define AVM_GENERIC(name, T) name##_##T

/// Refers to the base type in a function with a self parameter.
#define base (&self->_base)

#define AvmInvalid ((size_t)-1)

/// Signed 64-bit integer type.
typedef AVM_LONG_TYPE _long;

/// Unsigned 64-bit integer type.
typedef unsigned AVM_LONG_TYPE ulong;

/// Unsigned 32-bit integer type.
typedef unsigned int uint;

/// Unsigned 16-bit integer type.
typedef unsigned short ushort;

/// Unsigned 8-bit integer type.
typedef unsigned char byte;

/// An unknown object type.
typedef void* object;

/// An unknown function type.
typedef void (*AvmFunction)(void);

#ifdef AVM_MSVC
/// A type signifying that a function never returns.
#    define never __declspec(noreturn) void
#else
/// A type signifying that a function never returns.
#    define never _Noreturn void
#endif  // AVM_MSVC

#ifdef DOXYGEN
/// Primitive read-only string.
typedef char* str;

/// A type containing information about an object.
typedef struct AvmType AvmType;

/// A dynamic heap-allocated string.
typedef struct AvmString AvmString;
#else
/// Primitive read-only string.
typedef const char* str;

/// A type containing information about an object.
AVM_CLASS(AvmType, object, {
    str _name;
    size_t _size;
    AvmType* _baseType;
    AvmFunction _vptr[AVM_VFT_SIZE];
});

/// A dynamic heap-allocated string.
AVM_CLASS(AvmString, object, {
    uint _capacity;
    uint _length;
    char* _buffer;
});
#endif  // DOXYGEN

/// A weak pointer to a type T.
#define weakptr(T) T*

/// A pointer to a heap-allocated type T.
#define box(T) T*

/**
 * @brief Gets information about the type of an object.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The object instance.
 * @return The type information of the object.
 */
AVMAPI const AvmType* AvmObjectGetType(object self);

/**
 * @brief Compares two objects for equality.
 *
 * This function tries to use the FnEntryEquals virtual function entry. If no
 * such virtual function is available then the objects are compared
 * byte-by-byte.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p other must be not null.
 *
 * @param self The first object.
 * @param other The second object.
 *
 * @return true if the two objects are equal, otherwise false.
 */
AVMAPI bool AvmObjectEquals(object self, object other);

/**
 * @brief Destroys an object.
 *
 * This function tries to use the FnEntryDtor virtual function entry. If no
 * such virtual function then this function does nothing.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The object instance.
 */
AVMAPI void AvmObjectDestroy(object self);

/**
 * @brief Clones an object, creating an exact copy.
 *
 * This function tries to use the FnEntryClone virtual function entry. If no
 * such virtual function is available then a the object is simple copied to
 * heap memory.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The object instance.
 * @return The cloned object.
 */
AVMAPI object AvmObjectClone(object self);

/**
 * @brief Creates a string representation of an object.
 *
 * This function tries to use the FnEntryToString virtual function entry. If
 * no such virtual function is available then a default representation is
 * returned.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The object instance.
 * @return The string representation of the object.
 */
AVMAPI AvmString AvmObjectToString(object self);

#ifndef DOXYGEN
#    define AVM_TYPE_(T, B, ...)      \
        AvmType AVM_TI_NAME(T) = {    \
            ._type = typeid(AvmType), \
            ._vptr = __VA_ARGS__,     \
            ._name = #T,              \
            ._baseType = typeid(B),   \
            ._size = sizeof(T),       \
        }

// Ensure type size constraints.
static_assert_s(sizeof(_long) == AVM_LONG_SIZE);
static_assert_s(sizeof(ulong) == AVM_LONG_SIZE);
static_assert_s(sizeof(int) == AVM_INT_SIZE);
static_assert_s(sizeof(uint) == AVM_INT_SIZE);
static_assert_s(sizeof(short) == AVM_SHORT_SIZE);
static_assert_s(sizeof(ushort) == AVM_SHORT_SIZE);
static_assert_s(sizeof(char) == AVM_CHAR_SIZE);
static_assert_s(sizeof(byte) == AVM_BYTE_SIZE);
static_assert_s(sizeof(AvmString) == AVM_STRING_SIZE);

extern AvmType AVM_TI_NAME(AvmType);
extern AvmType AVM_TI_NAME(object);
extern AvmType AVM_TI_NAME(size_t);
extern AvmType AVM_TI_NAME(_long);
extern AvmType AVM_TI_NAME(ulong);
extern AvmType AVM_TI_NAME(int);
extern AvmType AVM_TI_NAME(uint);
extern AvmType AVM_TI_NAME(short);
extern AvmType AVM_TI_NAME(ushort);
extern AvmType AVM_TI_NAME(char);
extern AvmType AVM_TI_NAME(byte);
#endif  // DOXYGEN

#endif  // AVIUM_TYPES_H
