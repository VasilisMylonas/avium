/*
 * File: avium/core.h
 * Avium core utilities and types.
 *
 * About: Author
 * Vasilis Mylonas <vasilismylonas@protonmail.com>
 *
 * Section: License
 *
 * Copyright (C) 2021 Vasilis Mylonas
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

#ifndef AVIUM_CORE_H
#define AVIUM_CORE_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#include "avium/config.h"

/*
 * Section: Globals
 */

#ifndef static_assert
#    define static_assert _Static_assert
#endif  // static_assert

/*
 * Type: ptr
 * Signed pointer-sized integer type.
 */
typedef AVM_PTR_TYPE ptr;

/*
 * Type: uptr
 * Unsigned pointer-sized integer type.
 */
typedef unsigned AVM_PTR_TYPE uptr;

/*
 * Type: _long
 * Signed 64-bit integer type.
 */
typedef AVM_LONG_TYPE _long;

/*
 * Type: ulong
 * Unsigned 64-bit integer type.
 */
typedef unsigned AVM_LONG_TYPE ulong;

/*
 * Type: uint
 * Unsigned 32-bit integer type.
 */
typedef unsigned int uint;

/*
 * Type: ushort
 * Unsigned 16-bit integer type.
 */
typedef unsigned short ushort;

/*
 * Type: byte
 * Unsigned 8-bit integer type.
 */
typedef unsigned char byte;

/*
 * Type: str
 * Read-only string type.
 */
typedef const char* str;

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

/*
 * Macro: AVM_CONCAT
 * Concatenates two identifiers.
 *
 * Parameters:
 * x - The first identifier.
 * y - The second identifier.
 */
#define AVM_CONCAT(x, y) AVM_CONCAT_(x, y)

/*
 * Macro: AVM_STRINGIFY
 * Converts an identifier to a string.
 *
 * Parameters:
 * x - The identifier to stringify.
 */
#define AVM_STRINGIFY(x) AVM_STRINGIFY_(x)

/*
 * Macro: defer
 * Creates a scope in which an object will be available and be destroyed at
 * scope exit.
 *
 * Example:
 *
 * --- Code
 * defer(AvmString, s, AvmStringFrom("Hello World")) {
 *     AvmPrintf("%v\n", s);
 * } // <- 's' is destroyed here.
 * ---
 *
 * Parameters:
 * T - The type of the object.
 * name - The name of the variable.
 * init - The object initializer.
 */
#define defer(T, name, init) \
    for (T name = init; name != NULL; AvmDestroy(name), name = NULL)

/*
 * Macro: panic
 * Aborts execution, printing a message and location information.
 *
 * Parameters:
 * message - The message to print.
 */
#define panic(message) AvmPanic(message, __func__, __FILE__, __LINE__)

/*
 * Type: AvmFunction
 * An unknown function type.
 */
typedef void (*AvmFunction)(void);

/*
 * Type: never
 * A type signifying that a function never returns.
 */
#ifdef AVM_MSVC
#    define never __declspec(noreturn) void
#else

#    define never _Noreturn void
#endif  // AVM_MSVC

/*
 * Enum: AvmErrorKind
 * Describes the type of the error that occurred.
 *
 * EK_ARGUMENT          - An invalid argument was received by a function.
 * EK_OUT_OF_RANGE      - A provided index was out of range.
 * EK_OUT_OF_MEMORY     - There was not enough memory to handle an operation.
 * EK_INVALID_OPERATION - A function call was invalid for the current state.
 * EK_IO                - An IO error occurred.
 * EK_SYSTEM            - An unknown system error occurred.
 * EK_NOT_FOUND         - A required resource was unavailable.
 */
typedef enum {
    _EK_MIN = 16,
    EK_ARGUMENT,
    EK_OUT_OF_RANGE,
    EK_OUT_OF_MEMORY,
    EK_INVALID_OPERATION,
    EK_IO,
    EK_SYSTEM,
    EK_NOT_FOUND,
    _EK_MAX = 64,
} AvmErrorKind;

/*
 * Type: AvmType
 * A type containing information about an object.
 */
typedef struct _AvmType* AvmType;

/*
 * Type: AvmString
 * A dynamic string.
 */
typedef struct _AvmString* AvmString;

/*
 * Type: object
 * An unknown object type.
 */
typedef void* object;

/*
 * Function: AvmObjectType
 * Gets information about the type of an object.
 *
 * Parameters:
 * self - The object.
 *
 * Returns:
 * AvmType - The type information of the object.
 */
AVMAPI AvmType AvmObjectType(object self);

/*
 * Function: AvmObjectName
 * Gets the name of an object.
 *
 * Parameters:
 * self - The object.
 *
 * Returns:
 * str - The object's name.
 */
AVMAPI str AvmObjectName(object self);

/*
 * Function: AvmObjectSize
 * Gets the size of an object.
 *
 * Parameters:
 * self - The object.
 *
 * Returns:
 * size_t - The object's size.
 */
AVMAPI size_t AvmObjectSize(object self);

/*
 * Function: AvmObjectEq
 * Compares two objects for equality.
 *
 * Description:
 * This function tries to use the FUNC_EQ virtual function entry to compare
 * for equality. If no such virtual function is available then memcmp is used.
 *
 * Parameters:
 * lhs - The first object.
 * rhs - The second object.
 *
 * Returns:
 * bool - true if the two objects are equal, otherwise false.
 */
AVMAPI bool AvmObjectEq(object lhs, object rhs);

/*
 * Function: AvmDestroy
 * Destroys an object and deallocates its memory.
 *
 * Description:
 * This function tries to use the FUNC_DTOR virtual function entry to destroy
 * the object. If no such virtual function is available then free is used.
 *
 * Parameters:
 * self - The object.
 */
AVMAPI void AvmDestroy(object self);

/*
 * Function: AvmClone
 * Clones an object, creating an exact copy.
 *
 * Description:
 * This function tries to use the FUNC_CLONE virtual function entry to clone
 * the object. If no such virtual function is available then a combination of
 * malloc and memcpy is used.
 *
 * Parameters:
 * self - The object.
 *
 * Returns:
 * object - The cloned object.
 */
AVMAPI object AvmClone(object self);

/*
 * Function: AvmToString
 * Creates a string representation of an object.
 *
 * Description:
 * This function tries to use the FUNC_TO_STRING virtual function entry to
 * create a string representation of the object. If no such virtual function
 * is available then this function traps.
 *
 * Parameters:
 * self - The object.
 *
 * Returns:
 * AvmString - The string representation of the object.
 *
 * See also:
 * <AvmVirtualFunctionTrap>
 */
AVMAPI AvmString AvmToString(object self);

/*
 * Function: AvmGetLength
 * Gets the length of a container.
 *
 * Description:
 * This function tries to use the FUNC_GET_LENGTH virtual function entry to
 * get the length of the container. If no such virtual function is available
 * then this function traps.
 *
 * Parameters:
 * self - The container.
 *
 * Returns:
 * size_t - The length of the container.
 *
 * See also:
 * <AvmVirtualFunctionTrap>
 */
AVMAPI size_t AvmGetLength(object self);

/*
 * Function: AvmGetCapacity
 * Gets the capacity of a container.
 *
 * Description:
 * This function tries to use the FUNC_GET_CAPACITY virtual function entry to
 * get the capacity of the container. If no such virtual function is available
 * then this function traps.
 *
 * Parameters:
 * self - The container.
 *
 * Returns:
 * size_t - The capacity of the container.
 *
 * See also:
 * <AvmVirtualFunctionTrap>
 */
AVMAPI size_t AvmGetCapacity(object self);

/*
 * Function: AvmVirtualFunctionTrap
 * The trap function called when a virtual function is not implemented.
 *
 * Parameters:
 * function - The function name.
 * type - The object type.
 *
 * Returns:
 * never - This function never returns.
 */
AVMAPI never AvmVirtualFunctionTrap(str function, AvmType type);

/*
 * Function: AvmPanic
 * Aborts execution, printing a message and location information.
 *
 * Parameters:
 * message - The message to be printed.
 * function - The function name.
 * file - The file name.
 * line - The line number.
 *
 * Returns:
 * never - This function never returns.
 */
AVMAPI never AvmPanic(str message, str function, str file, uint line);

/*
 * Function: AvmMemCopy
 * Copies memory from one block to another. This will copy length bytes from
 * source to destination, but not more than size.
 *
 * Parameters:
 * source - The memory block to copy from.
 * length - The length of the source buffer.
 * destination - The memory block to copy to.
 * size - The size of the destination buffer.
 */
AVMAPI void AvmMemCopy(byte* source, size_t length, byte* destination,
                       size_t size);

/*
 * Struct: AvmResult
 * A type indicating successful or unsuccessful function execution.
 *
 * See also:
 * <AvmSuccess>
 * <AvmFailure>
 */
typedef union {
    AvmErrorKind kind;
    object value;
} AvmResult;

/*
 * Function: AvmSuccess
 * Creates an AvmResult indicating success.
 *
 * Parameters:
 * value - The value to be returned along the AvmResult.
 *
 * Returns:
 * AvmResult - An AvmResult indicating success.
 *
 * See also:
 * <AvmFailure>
 * <AvmIsFailure>
 *
 * Note:
 * This function is inline.
 */
inline AvmResult AvmSuccess(object value) {
    return (AvmResult){.value = value};
}

/*
 * Function: AvmFailure
 * Creates an AvmResult indicating failure.
 *
 * Parameters:
 * kind - The type of failure that occurred.
 *
 * Returns:
 * AvmResult - An AvmResult indicating failure.
 *
 * See also:
 * <AvmSuccess>
 * <AvmIsFailure>
 *
 * Note:
 * This function is inline.
 */
inline AvmResult AvmFailure(AvmErrorKind kind) {
    if (kind <= _EK_MIN || kind >= _EK_MAX) {
        panic("Parameter `kind` was invalid. Do not use _EK_MIN and _EK_MAX.");
    }

    return (AvmResult){.kind = kind};
}

/*
 * Function: AvmIsFailure
 * Determines whether an AvmResult describes a failure.
 *
 * Parameters:
 * self - The AvmResult.
 *
 * Returns:
 * bool - true if the AvmResult represents failure, otherwise false.
 *
 * See also:
 * <AvmSuccess>
 * <AvmFailure>
 *
 * Note:
 * This function is inline.
 */
inline bool AvmIsFailure(AvmResult self) {
    return self.kind > _EK_MIN && self.kind < _EK_MAX;
}

/*
 * Function: AvmUnwrap
 * Retrieves the value from an AvmResult, panicking if it represents an Error.
 *
 * Parameters:
 * self - The AvmResult.
 *
 * Returns:
 * object - The object contained in the AvmResult.
 *
 * See also:
 * <AvmSuccess>
 * <AvmFailure>
 *
 * Note:
 * This function is inline.
 */
inline object AvmUnwrap(AvmResult self) {
    if (AvmIsFailure(self)) {
        panic("Tried to unwrap a result describing failure.");
    }

    return self.value;
}

/*
 * Struct: AvmOptional
 * A type which may contain a value.
 */
typedef uptr AvmOptional;

/*
 * Function: AvmSome
 * Creates an AvmOptional with the specified value.
 *
 * Parameters:
 * value - The object to be contained in the AvmOptional.
 *
 * Returns:
 * AvmOptional - The created AvmOptional.
 *
 * Note:
 * This function is inline.
 */
inline AvmOptional AvmSome(object value) {
    if (((uptr)value) == AVM_OPTIONAL_NONE) {
        panic("Tried to create an AvmOptional with an invalid value.");
    }
    return (AvmOptional)value;
}

/*
 * Function: AvmNone
 * Creates an AvmOptional with the `none` value.
 *
 * Returns:
 * AvmOptional - The created AvmOptional.
 *
 * Note:
 * This function is inline.
 */
inline AvmOptional AvmNone() { return (AvmOptional)AVM_OPTIONAL_NONE; }

/*
 * Function: AvmHasValue
 * Determines whether an AvmOptional contains a value.
 *
 * Returns:
 * bool - true if the optional contains a value, otherwise false.
 *
 * Note:
 * This function is inline.
 */
inline bool AvmHasValue(AvmOptional optional) {
    return optional != AVM_OPTIONAL_NONE;
}

/*
 * Struct: AvmVersion
 * A type representing a software version in the format:
 * <major>.<minor>.<patch>-<tag>
 */
typedef struct _AvmVersion* AvmVersion;

/*
 * Macro: AvmVersion
 * Shorthand for AvmVersion_ctor.
 */
#define AvmVersion(major, minor, patch, tag) \
    AvmVersion_ctor(major, minor, patch, tag)

/*
 * Function: AvmVersion_ctor
 * Creates a new AvmVersion instance.
 *
 * Parameters:
 * major - The version major number (incremented at breaking changes).
 * minor - The version minor number (incremented at non-breaking changes).
 * patch - The version patch number (incremented at bug fixes).
 * tag - The version tag (a, b, etc).
 *
 * Returns:
 * AvmVersion - The created AvmVersion.
 */
AVMAPI AvmVersion AvmVersion_ctor(uint major, uint minor, uint patch, char tag);

/*
 * Function: AvmVersionIsCompatible
 * Determines whether 2 AvmVersion instances are compatible (whether the 2
 * major numbers are equal).
 *
 * Parameters:
 * self - The first AvmVersion instance.
 * other - The second AvmVersion instance.
 *
 * Returns:
 * bool - true if the versions are compatible, otherwise false.
 */
AVMAPI bool AvmVersionIsCompatible(AvmVersion self, AvmVersion other);

/*
 * Function: AvmVersionGetMajor
 * Gets the major number of an AvmVersion instance.
 *
 * Parameters:
 * self - The AvmVersion instance.
 *
 * Returns:
 * uint - The major number.
 */
AVMAPI uint AvmVersionGetMajor(AvmVersion self);

/*
 * Function: AvmVersionGetMinor
 * Gets the minor number of an AvmVersion instance.
 *
 * Parameters:
 * self - The AvmVersion instance.
 *
 * Returns:
 * uint - The minor number.
 */
AVMAPI uint AvmVersionGetMinor(AvmVersion self);

/*
 * Function: AvmVersionGetPatch
 * Gets the patch number of an AvmVersion instance.
 *
 * Parameters:
 * self - The AvmVersion instance.
 *
 * Returns:
 * uint - The patch number.
 */
AVMAPI uint AvmVersionGetPatch(AvmVersion self);

/*
 * Function: AvmVersionGetTag
 * Gets the version tag of an AvmVersion instance.
 *
 * Parameters:
 * self - The AvmVersion instance.
 *
 * Returns:
 * uint - The version tag number.
 */
AVMAPI char AvmVersionGetTag(AvmVersion self);

#endif  // AVIUM_CORE_H
