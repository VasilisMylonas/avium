/**
 * @file avium/core.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Avium core utilities and types.
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

#ifndef AVIUM_CORE_H
#define AVIUM_CORE_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#include "avium/exports.h"
#include "avium/config.h"

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

/// @}

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
 * @brief Creates a scope in which an object will be available and be destroyed
 *        at scope exit.
 *
 * @param T The type of the object.
 * @param name The name of the variable.
 * @param init The object initializer.
 */
#define defer(T, name, init) \
    for (T name = init; name != NULL; AvmDestroy(name), name = NULL)

/**
 * @brief Aborts execution, printing a message and location information.
 *
 * @param message The message to print.
 */
#define panic(message) AvmPanic(message, __func__, __FILE__, __LINE__)

/// An unknown function type.
typedef void (*AvmFunction)(void);

#ifdef AVM_MSVC
/// A type signifying that a function never returns.
#    define never __declspec(noreturn) void
#else
/// A type signifying that a function never returns.
#    define never _Noreturn void
#endif  // AVM_MSVC

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

/// A type containing information about an object.
typedef struct AvmType* AvmType;

/// A dynamic string.
typedef struct AvmString* AvmString;

/// An unknown object type.
typedef void* object;

/**
 * @brief Gets information about the type of an object.
 *
 * @param self The object.
 * @return AvmType The type information of the object.
 */
AVMAPI AvmType AvmObjectType(object self);

/**
 * @brief Gets the name of an object.
 *
 * @param self The object.
 * @return str The object's name.
 */
AVMAPI str AvmObjectName(object self);

/**
 * @brief Gets the size of an object.
 *
 * @param self The object.
 * @return size_t The object's size.
 */
AVMAPI size_t AvmObjectSize(object self);

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
AVMAPI bool AvmObjectEq(object lhs, object rhs);

/**
 * @brief Destroys an object and deallocates its memory.
 *
 * This function tries to use the FUNC_DTOR virtual function entry to destroy
 * the object. If no such virtual function is available then free is used.
 *
 * @param self The object.
 */
AVMAPI void AvmDestroy(object self);

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
AVMAPI object AvmClone(object self);

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
AVMAPI AvmString AvmToString(object self);

/**
 * @brief Gets the length of a container.
 *
 * This function tries to use the FUNC_GET_LENGTH virtual function entry to
 * get the length of the container. If no such virtual function is available
 * then this function traps.
 *
 * @param self The container.
 * @return size_t The length of the container.
 *
 * @see AvmVirtualFunctionTrap
 */
AVMAPI size_t AvmGetLength(object self);

/**
 * @brief Gets the capacity of a container.
 *
 * This function tries to use the FUNC_GET_CAPACITY virtual function entry to
 * get the capacity of the container. If no such virtual function is available
 * then this function traps.
 *
 * @param self The container.
 * @return size_t The capacity of the container.
 *
 * @see AvmVirtualFunctionTrap
 */
AVMAPI size_t AvmGetCapacity(object self);

/**
 * @brief The trap function called when a virtual function is not implemented.
 *
 * @param function The function name.
 * @param type The object type.
 *
 * @return never This function never returns.
 */
AVMAPI never AvmVirtualFunctionTrap(str function, AvmType type);

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

/**
 * @brief Copies memory from one block to another.
 *
 * This will copy length bytes from source to destination, but not more than
 * size.
 *
 * @param source The memory block to copy from.
 * @param length The length of the source buffer.
 * @param destination The memory block to copy to.
 * @param size The size of the destination buffer.
 */
AVMAPI void AvmMemCopy(byte* source, size_t length, byte* destination,
                       size_t size);

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

// A type representing a software version in the format: major.minor.patch-tag
typedef struct AvmVersion* AvmVersion;

/// Shorthand for AvmVersion_ctor.
#define AvmVersion(major, minor, patch, tag) \
    AvmVersion_ctor(major, minor, patch, tag)

/**
 * @brief Creates a new AvmVersion instance.
 *
 * @param major The version major number (incremented at breaking changes).
 * @param minor The version minor number (incremented at non-breaking changes).
 * @param patch The version patch number (incremented at bug fixes).
 * @param tag The version tag (a, b, etc).
 *
 * @return AvmVersion The created AvmVersion.
 */
AVMAPI AvmVersion AvmVersion_ctor(uint major, uint minor, uint patch, char tag);

/**
 * @brief Determines whether 2 AvmVersion instances are compatible (whether the
 * 2 major numbers are equal).
 *
 * @param self The first AvmVersion instance.
 * @param other The second AvmVersion instance.
 *
 * @return true The versions are compatible.
 * @return false The versions are not compatible.
 */
AVMAPI bool AvmVersionIsCompatible(AvmVersion self, AvmVersion other);

/**
 * @brief Gets the major number of an AvmVersion instance.
 *
 * @param self The AvmVersion instance.
 * @return uint The major number.
 */
AVMAPI uint AvmVersionGetMajor(AvmVersion self);

/**
 * @brief Gets the minor number of an AvmVersion instance.
 *
 * @param self The AvmVersion instance.
 * @return uint The minor number.
 */
AVMAPI uint AvmVersionGetMinor(AvmVersion self);

/**
 * @brief Gets the patch number of an AvmVersion instance.
 *
 * @param self The AvmVersion instance.
 * @return uint The patch number.
 */
AVMAPI uint AvmVersionGetPatch(AvmVersion self);

/**
 * @brief Gets the version tag of an AvmVersion instance.
 *
 * @param self The AvmVersion instance.
 * @return char The version tag number.
 */
AVMAPI char AvmVersionGetTag(AvmVersion self);

/// Represents a command line option.
typedef struct {
    /// The long option in the form of --help, but without the --.
    str longOption;

    /// The short option in the form of -h, but without the -.
    char shortOption;
} AvmOption;

/**
 * @brief Determines whether the specified option is present.
 *
 * @param argc The argc parameter from main.
 * @param argv The argv parameter from main.
 * @param option The desired option.
 *
 * @return true The specified option was present.
 * @return false The specified option was not present.
 */
AVMAPI bool AvmHasOption(int argc, str* argv, AvmOption option);

/**
 * @brief Retrieves a command line option.
 *
 * @param argc The argc parameter from main.
 * @param argv The argv parameter from main.
 * @param option The desired option.
 *
 * @return AvmSome(str) The command line option value.
 * @return AvmNone The option was not present.
 */
AVMAPI AvmOptional AvmGetOption(int argc, str* argv, AvmOption option);

#endif  // AVIUM_CORE_H
