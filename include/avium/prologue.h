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

/// A dynamic string.
typedef struct AvmString* AvmString;

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
    /// No error occurred.
    EK_NONE = 0,

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
} AvmErrorKind;

/// A type indicating successful or unsuccessful function execution.
typedef struct {
    AvmErrorKind _kind;
    union {
        object _value;
        str _error;
    };
} AvmResult;

/**
 * @brief Creates an AvmResult indicating success.
 *
 * @param value The value to be returned along the AvmResult.
 * @return AvmResult An AvmResult indicating success.
 *
 * @note This function is inline.
 */
AVMAPI inline AvmResult AvmSuccess(object value) {
    return (AvmResult){._kind = EK_NONE, ._value = value};
}

/**
 * @brief Creates an AvmResult indicating failure.
 *
 * @param kind The type of failure that occurred.
 * @param error A string representing the error that occurred.
 * @return AvmResult An AvmResult indicating failure.
 *
 * @note This function is inline.
 */
AVMAPI inline AvmResult AvmFailure(AvmErrorKind kind, str error) {
    if (kind == EK_NONE) {
        panic(
            "Tried to create an `AvmResult` using `EK_NONE`. To create an "
            "`AvmResult` describing success, use `AvmSuccess` instead.");
    }

    if (error == NULL) {
        panic("Parameter `error` was `NULL`.");
    }

    return (AvmResult){._kind = kind, ._error = error};
}

AVMAPI inline str AvmResultGetError(AvmResult self) {
    if (self._kind == EK_NONE) {
        panic(
            "Tried to get an error from an `AvmResult` describing success. "
            "Check "
            "with `AvmResultIsFailure` first.");
    }

    return self._error;
}

/**
 * @brief Determines whether an AvmResult describes a failure.
 *
 * @param self The AvmResult.
 * @return true The AvmResult represents failure.
 * @return false The AvmResult contains a result.
 *
 * @note This function is inline.
 */
AVMAPI inline bool AvmResultIsFailure(AvmResult self) {
    return self._kind != EK_NONE;
}

/**
 * @brief Retrieves the value from an AvmResult, panicking if it represents an
 * Error.
 *
 * @param self The AvmResult.
 * @return object The object contained in the AvmResult.
 *
 * @note This function is inline.
 */
AVMAPI inline object AvmResultUnwrap(AvmResult self) {
    if (AvmResultIsFailure(self)) {
        panic(
            "Tried to unwrap an `AvmResult` describing failure. Check with "
            "`AvmResultIsFailure` first.");
    }

    return self._value;
}

/// A type which may contain a value.
typedef struct {
    object _value;
    bool _hasValue;
} AvmOptional;

/**
 * @brief Creates an AvmOptional with the `none` value.
 *
 * @return AvmOptional The created AvmOptional.
 * @note This function is inline.
 */
AVMAPI inline AvmOptional AvmNone() {
    return (AvmOptional){._value = 0, ._hasValue = false};
}

/**
 * @brief Creates an AvmOptional with the specified value.
 *
 * @param value The object to be contained in the AvmOptional.
 * @return AvmOptional The created AvmOptional.
 *
 * @note This function is inline.
 */
AVMAPI inline AvmOptional AvmSome(object value) {
    return (AvmOptional){._value = value, ._hasValue = true};
}

/**
 * @brief Determines whether an AvmOptional contains a value.
 *
 * @return true if the optional contains a value, otherwise false.
 *
 * @note This function is inline.
 */
AVMAPI inline bool AvmOptionalHasValue(AvmOptional optional) {
    return optional._hasValue;
}

/**
 * @brief Creates a scope in which an object will be available and be destroyed
 *        at scope exit.
 *
 * @param T The type of the object.
 * @param name The name of the variable.
 * @param init The object initializer.
 */
#define defer(T, name, init) \
    for (T name = init; name != NULL; AvmObjectDestroy(name), name = NULL)

/**
 * @brief Compares two objects for equality.
 *
 * This function tries to use the FUNC_EQ virtual function entry to compare
 * for equality. If no such virtual function is available then memcmp is used.
 *
 * @param lhs The first object.
 * @param rhs The second object.
 *
 * @return true The two objects are equal.
 * @return false The two objects are not equal.
 */
AVMAPI bool AvmObjectEquals(object lhs, object rhs);

/**
 * @brief Destroys an object and deallocates its memory.
 *
 * This function tries to use the FUNC_DTOR virtual function entry to destroy
 * the object. If no such virtual function is available then free is used.
 *
 * @param self The object.
 */
AVMAPI void AvmObjectDestroy(object self);

/**
 * @brief Clones an object, creating an exact copy.
 *
 * This function tries to use the FUNC_CLONE virtual function entry to clone
 * the object. If no such virtual function is available then a combination of
 * malloc and memcpy is used.
 *
 * @param self The object.
 * @return object The cloned object.
 */
AVMAPI object AvmObjectClone(object self);

/**
 * @brief Creates a string representation of an object.
 *
 * This function tries to use the FUNC_TO_STRING virtual function entry to
 * create a string representation of the object. If no such virtual function
 * is available then this function traps.
 *
 * @param self The object.
 * @return AvmString The string representation of the object.
 *
 * @see AvmVirtualFunctionTrap
 */
AVMAPI AvmString AvmObjectToString(object self);

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

static_assert(sizeof(AvmResult) == 16, "");
static_assert(sizeof(AvmOptional) == 16, "");

#define AVM_CONCAT_(x, y) x##y
#define AVM_STRINGIFY_(x) #x

#endif  // AVIUM_PROLOGUE_H
