/**
 * @file avium/prologue.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief The Avium prologue.
 * @version 0.1
 * @date 2021-03-17
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef AVIUM_PROLOGUE_H
#define AVIUM_PROLOGUE_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include "avium/config.h"
#include "avium/exports.h"

/**
 * @brief Concatenates two identifiers.
 *
 * @param x The first identifier.
 * @param y The second identifier.
 */
#define AVM_CONCAT(x, y) AVM_CONCAT_(x, y)

/**
 * @brief Converts an identifier to a string.
 *
 * @param x The identifier to stringify.
 */
#define AVM_STRINGIFY(x) AVM_STRINGIFY_(x)

/**
 * @brief Aborts execution, printing a message and location information.
 *
 * @param message The message to print.
 */
#define panic(message) AvmPanic(message, __func__, __FILE__, __LINE__)

/**
 * @defgroup avm_core_types Core primitive types.
 *
 * @{
 */

/// Signed pointer-sized integer type.
typedef AVM_PTR_TYPE ptr;

/// Unsigned pointer-sized integer type.
typedef unsigned AVM_PTR_TYPE uptr;

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

/// @}

/**
 * @brief Aborts execution, printing a message and location information.
 *
 * @param message The message to be printed.
 * @param function The function name.
 * @param file The file name.
 * @param line The line number.
 *
 * @return never This function never returns.
 */
AVMAPI never AvmPanic(str message, str function, str file, uint line);

/// Describes the type of the error that occurred.
typedef enum {
    /// Internal constant, do not use.
    _EK_MIN = 16,

    /// An invalid argument was received by a function.
    EK_ARGUMENT,

    /// A provided index was out of range.
    EK_OUT_OF_RANGE,

    /// There was not enough memory to handle an operation.
    EK_OUT_OF_MEMORY,

    /// A function call was invalid for the current state.
    EK_INVALID_OPERATION,

    /// An IO error occurred.
    EK_IO,

    ///  An unknown system error occurred.
    EK_SYSTEM,

    /// A required resource was unavailable.
    EK_NOT_FOUND,

    /// Internal constant, do not use.
    _EK_MAX = 64,
} AvmErrorKind;

/**
 * @brief A type indicating successful or unsuccessful function execution.
 *
 * @see AvmSuccess AvmFailure
 */
typedef union {
    /// The type of the error that occurred.
    AvmErrorKind kind;

    /// The object result.
    object value;
} AvmResult;

/**
 * @brief Creates an AvmResult indicating success.
 *
 * @param value The value to be returned along the AvmResult.
 * @return AvmResult An AvmResult indicating success.
 *
 * @see AvmFailure AvmIsFailure
 *
 * @note This function is inline.
 */
inline AvmResult AvmSuccess(object value) {
    return (AvmResult){.value = value};
}

/**
 * @brief Creates an AvmResult indicating failure.
 *
 * @param kind The type of failure that occurred.
 * @return AvmResult An AvmResult indicating failure.
 *
 * @see AvmSuccess AvmIsFailure
 *
 * @note This function is inline.
 */
inline AvmResult AvmFailure(AvmErrorKind kind) {
    if (kind <= _EK_MIN || kind >= _EK_MAX) {
        panic("Parameter `kind` was invalid. Do not use _EK_MIN and _EK_MAX.");
    }

    return (AvmResult){.kind = kind};
}

/**
 * @brief Determines whether an AvmResult describes a failure.
 *
 * @param self The AvmResult.
 * @return true The AvmResult represents failure.
 * @return false The AvmResult contains a result.
 *
 * @see AvmSuccess AvmFailure
 *
 * @note This function is inline.
 */
inline bool AvmIsFailure(AvmResult self) {
    return self.kind > _EK_MIN && self.kind < _EK_MAX;
}

/**
 * @brief Retrieves the value from an AvmResult, panicking if it represents an
 * Error.
 *
 * @param self The AvmResult.
 * @return object The object contained in the AvmResult.
 *
 * @see AvmSuccess AvmFailure
 *
 * @note This function is inline.
 */
inline object AvmUnwrap(AvmResult self) {
    if (AvmIsFailure(self)) {
        panic("Tried to unwrap a result describing failure.");
    }

    return self.value;
}

/// A type which may contain a value.
typedef uptr AvmOptional;

/**
 * @brief Creates an AvmOptional with the specified value.
 *
 * @param value The object to be contained in the AvmOptional.
 * @return AvmOptional The created AvmOptional.
 *
 * @note This function is inline.
 */
inline AvmOptional AvmSome(object value) {
    if (((uptr)value) == AVM_OPTIONAL_NONE) {
        panic("Tried to create an AvmOptional with an invalid value.");
    }
    return (AvmOptional)value;
}

/**
 * @brief Creates an AvmOptional with the `none` value.
 *
 * @return AvmOptional The created AvmOptional.
 *
 * @note This function is inline.
 */
inline AvmOptional AvmNone() { return (AvmOptional)AVM_OPTIONAL_NONE; }

/**
 * @brief Determines whether an AvmOptional contains a value.
 *
 * @return true if the optional contains a value, otherwise false.
 *
 * @note This function is inline.
 */
inline bool AvmHasValue(AvmOptional optional) {
    return optional != AVM_OPTIONAL_NONE;
}

// Ensure correct type sizes.
static_assert(sizeof(ptr) == sizeof(void*), "");
static_assert(sizeof(uptr) == sizeof(void*), "");
static_assert(sizeof(_long) == 8, "");
static_assert(sizeof(ulong) == 8, "");
static_assert(sizeof(int) == 4, "");
static_assert(sizeof(uint) == 4, "");
static_assert(sizeof(short) == 2, "");
static_assert(sizeof(ushort) == 2, "");
static_assert(sizeof(char) == 1, "");
static_assert(sizeof(byte) == 1, "");

#define AVM_CONCAT_(x, y) x##y
#define AVM_STRINGIFY_(x) #x

#endif  // AVIUM_PROLOGUE_H
