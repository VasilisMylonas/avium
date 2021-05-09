/**
 * @file avium/types.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Avium core types.
 * @version 0.2.5
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

#ifndef AVIUM_TYPES_H
#define AVIUM_TYPES_H

#include "avium/config.h"
#include "avium/exports.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

typedef AVM_LONG_TYPE _long;          ///< Signed 64-bit integer type.
typedef unsigned AVM_LONG_TYPE ulong; ///< Unsigned 64-bit integer type.
typedef unsigned int uint;            ///< Unsigned 32-bit integer type.
typedef unsigned short ushort;        ///< Unsigned 16-bit integer type.
typedef unsigned char byte;           ///< Unsigned 8-bit integer type.
typedef void *object;                 ///< An unknown object type.
typedef void (*AvmFunction)(void);    ///< An unknown function type.
typedef const char *str;              ///< Primitive read-only string.
#define weakptr(T) T *                ///< A weak pointer to a type T.
#define box(T) T *                    ///< A pointer to a heap-allocated type T.

/**
 * @brief Creates an Avium class type.
 *
 * @param T The name of the type.
 * @param B The base class of the type.
 * @param ... Member declaration in braces ({ ... })
 */
#define AVM_CLASS(T, B, ...)             \
    typedef struct T T;                  \
    extern const AvmType AVM_TI_NAME(T); \
    struct T                             \
    {                                    \
        union                            \
        {                                \
            const AvmType *_type;        \
            B _base;                     \
        };                               \
        struct __VA_ARGS__;              \
    }

/**
 * @brief Generates type info for a type.
 *
 * @param T The type for which to generate type info.
 * @param B The base type.
 * @param ... The type vtable enclosed in braces ({...})
 */
#define AVM_TYPE(T, B, ...)          \
    const AvmType AVM_TI_NAME(T) = { \
        ._type = typeid(AvmType),    \
        ._vptr = __VA_ARGS__,        \
        ._name = #T,                 \
        ._baseType = typeid(B),      \
        ._size = sizeof(T),          \
    }

/**
 * @brief Creates an Avium interface type.
 *
 * @param T The name of the type.
 */
#define AVM_INTERFACE(T)      \
    typedef struct T T;       \
    struct T                  \
    {                         \
        const AvmType *_type; \
    }

#define AVM_FORWARD_TYPE(T) typedef struct T T

#ifdef AVM_MSVC
#define typeof(T) decltype(T)
#else
#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wkeyword-macro"
#endif
#define typeof(T) __typeof__(T)
#ifdef __clang__
#pragma GCC diagnostic pop
#endif
#endif

#ifndef DOXYGEN
AVM_FORWARD_TYPE(AvmType);
AVM_FORWARD_TYPE(AvmError);
AVM_FORWARD_TYPE(AvmString);

static_assert_s(sizeof(_long) == AVM_LONG_SIZE);
extern const AvmType AVM_TI_NAME(_long);

static_assert_s(sizeof(ulong) == AVM_LONG_SIZE);
extern const AvmType AVM_TI_NAME(ulong);

static_assert_s(sizeof(int) == AVM_INT_SIZE);
extern const AvmType AVM_TI_NAME(int);

static_assert_s(sizeof(uint) == AVM_INT_SIZE);
extern const AvmType AVM_TI_NAME(uint);

static_assert_s(sizeof(short) == AVM_SHORT_SIZE);
extern const AvmType AVM_TI_NAME(short);

static_assert_s(sizeof(ushort) == AVM_SHORT_SIZE);
extern const AvmType AVM_TI_NAME(ushort);

static_assert_s(sizeof(char) == AVM_CHAR_SIZE);
extern const AvmType AVM_TI_NAME(char);

static_assert_s(sizeof(byte) == AVM_BYTE_SIZE);
extern const AvmType AVM_TI_NAME(byte);

static_assert_s(sizeof(object) == AVM_OBJECT_SIZE);
extern const AvmType AVM_TI_NAME(object);
#endif

#endif // AVIUM_TYPES_H
