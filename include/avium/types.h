/**
 * @file avium/types.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Primitive Avium types and related functions.
 * @version 0.2
 * @date 2021-04-14
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
#define AVM_CLASS(T, B, ...)      \
    typedef struct T T;           \
    struct T {                    \
        union {                   \
            const AvmType* _type; \
            B _base;              \
        };                        \
        struct __VA_ARGS__;       \
    }

/**
 * @brief Creates an Avium interface type.
 *
 * @param T The name of the type.
 */
#define AVM_INTERFACE(T)      \
    typedef struct T T;       \
    struct T {                \
        const AvmType* _type; \
    }

/// Convieniece macro for type-generic types.
#define AVM_GENERIC(name, T) name##_##T

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

/// Read-only string type.
typedef const char* str;

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

/// A type containing information about an object.
typedef struct AvmType AvmType;

/// A dynamic string.
AVM_CLASS(AvmString, object, {
    size_t _capacity;
    size_t _length;
    char* _buffer;
});

/**
 * @brief Gets information about the type of an object.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The object instance.
 * @return The type information of the object.
 */
AVMAPI AvmType* AvmObjectGetType(object self);

/**
 * @brief Compares two objects for equality.
 *
 * This function tries to use the FUNC_EQ virtual function entry to compare
 * for equality. If no such virtual function is available then the objects are
 * compared byte-by-byte.
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
 * @brief Destroys an object and deallocates its memory.
 *
 * This function tries to use the FUNC_DTOR virtual function entry to destroy
 * the object. If no such virtual function then this function does nothing.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The object instance.
 */
AVMAPI void AvmObjectDestroy(object self);

/**
 * @brief Clones an object, creating an exact copy.
 *
 * This function tries to use the FUNC_CLONE virtual function entry to clone
 * the object. If no such virtual function is available then a the object is
 * simple copied to heap memory.
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
 * This function tries to use the FUNC_TO_STRING virtual function entry to
 * create a string representation of the object. If no such virtual function
 * is available then a default representation is returned.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The object instance.
 * @return The string representation of the object.
 */
AVMAPI AvmString AvmObjectToString(object self);

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

#endif  // AVIUM_TYPES_H
