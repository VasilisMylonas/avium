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

/// A weak pointer to a type T.
#define weakptr(T) T *

/// A pointer to a heap-allocated type T.
#define box(T) T *

typedef struct AvmType AvmType;

#ifdef AVM_MSVC
/// A type signifying that a function never returns.
#define never __declspec(noreturn) void
#else
/// A type signifying that a function never returns.
#define never _Noreturn void
#endif // AVM_MSVC

extern const AvmType AVM_TI_NAME(AvmType);
extern const AvmType AVM_TI_NAME(object);
extern const AvmType AVM_TI_NAME(size_t);
extern const AvmType AVM_TI_NAME(_long);
extern const AvmType AVM_TI_NAME(ulong);
extern const AvmType AVM_TI_NAME(int);
extern const AvmType AVM_TI_NAME(uint);
extern const AvmType AVM_TI_NAME(short);
extern const AvmType AVM_TI_NAME(ushort);
extern const AvmType AVM_TI_NAME(char);
extern const AvmType AVM_TI_NAME(byte);

#endif // AVIUM_TYPES_H
