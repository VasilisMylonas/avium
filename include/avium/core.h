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
#include <stdint.h>

#include "avium/config.h"

#define AVM_CONCAT_(x, y) x##y
#define AVM_STRINGIFY_(x) #x

/*
 * Macro: AVM_CONCAT
 * Concatenates two identifier.
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

#define defer(x)                                           \
    for (size_t AVM_CONCAT(avmDeferCounter, __LINE__) = 0; \
         AVM_CONCAT(avmDeferCounter, __LINE__) < 1;        \
         AVM_CONCAT(avmDeferCounter, __LINE__)++, AvmDestroy(x))

/*
 * Type: char8_t
 * An 8 bit character type.
 */
typedef char char8_t;

/*
 * Type: function_t
 * An unknown function type.
 */
typedef void (*function_t)(void);

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
    EK_ARGUMENT = 1,
    EK_OUT_OF_RANGE,
    EK_OUT_OF_MEMORY,
    EK_INVALID_OPERATION,
    EK_IO,
    EK_SYSTEM,
    EK_NOT_FOUND,
} AvmErrorKind;

/*
 * Type: Type
 * A type containing information about an object.
 */
typedef struct Type* Type;

/*
 * Type: AvmString
 * A dynamic string.
 */
typedef struct AvmString* AvmString;

/*
 * Type: object_t
 * An unknown object type.
 */
typedef void* object_t;

/*
 * Function: AvmObjectType
 * Gets information about the type of an object.
 *
 * Parameters:
 * self - The object.
 *
 * Returns:
 * Type - The type information of the object.
 */
AVMAPI Type AvmObjectType(object_t self);

/*
 * Function: AvmObjectName
 * Gets the name of an object.
 *
 * Parameters:
 * self - The object.
 *
 * Returns:
 * const char8_t* - The object's name.
 */
AVMAPI const char8_t* AvmObjectName(object_t self);

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
AVMAPI size_t AvmObjectSize(object_t self);

/*
 * Function: AvmObjectEq
 * Compares two objects for equality.
 *
 * Parameters:
 * lhs - The first object.
 * rhs - The second object.
 *
 * Returns:
 * bool - true if the two objects are equal, otherwise false.
 */
AVMAPI bool AvmObjectEq(object_t lhs, object_t rhs);

/*
 * Function: AvmDestroy
 * Destroys and object and deallocates its memory.
 *
 * Parameters:
 * object - The object.
 */
AVMAPI void AvmDestroy(object_t object);

/*
 * Function: AvmClone
 * Clones an object, creating an exact copy.
 *
 * Parameters:
 * object - The object.
 *
 * Returns:
 * object_t - The cloned object.
 */
AVMAPI object_t AvmClone(object_t object);

/*
 * Function: AvmToString
 * Creates a string representation of an object.
 *
 * Parameters:
 * object - The object.
 *
 * Returns:
 * AvmString - The string representation of the object.
 */
AVMAPI AvmString AvmToString(object_t object);

/*
 * Function: AvmGetLength
 * Gets the length of a container.
 *
 * Parameters:
 * object - The container.
 *
 * Returns:
 * size_t - The length of the container.
 */
AVMAPI size_t AvmGetLength(object_t object);

/*
 * Function: AvmGetCapacity
 * Gets the capacity of a container.
 *
 * Parameters:
 * object - The container.
 *
 * Returns:
 * size_t - The capacity of the container.
 */
AVMAPI size_t AvmGetCapacity(object_t object);

/*
 * Function: AvmVirtualFunctionTrap
 * A trap function called when a virtual function is not implemented.
 *
 * Parameters:
 * function - The function name.
 * type - The object type.
 *
 * Returns:
 * never - This function never returns.
 */
AVMAPI never AvmVirtualFunctionTrap(const char8_t* function, Type type);

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
AVMAPI never AvmPanic(const char8_t* message, const char8_t* function,
                      const char8_t* file, uint32_t line);

/*
 * Macro: panic
 * Aborts execution, printing a message and location information.
 *
 * Parameters:
 * message - The message to print.
 */
#define panic(message) AvmPanic(message, __func__, __FILE__, __LINE__)

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
    object_t value;
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
inline AvmResult AvmSuccess(object_t value) {
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
    return ((uintptr_t)self.value) <= UINT8_MAX && self.value != NULL;
}

/*
 * Function: AvmUnwrap
 * Retrieves the value from an AvmResult, panicking if it represents an Error.
 *
 * Parameters:
 * self - The AvmResult.
 *
 * Returns:
 * object_t - The object contained in the AvmResult.
 *
 * See also:
 * <AvmSuccess>
 * <AvmFailure>
 *
 * Note:
 * This function is inline.
 */
inline object_t AvmUnwrap(AvmResult self) {
    if (AvmIsFailure(self)) {
        panic("Tried to unwrap a result describing failure.");
    }

    return self.value;
}

/*
 * Struct: AvmOptional
 * A type which may contain a value.
 */
typedef uintptr_t AvmOptional;

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
inline AvmOptional AvmSome(object_t value) {
    if (((uintptr_t)value) == AVM_OPTIONAL_NONE) {
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
AVMAPI void AvmMemCopy(uint8_t* source, size_t length, uint8_t* destination,
                       size_t size);

typedef struct AvmVersion* AvmVersion;

#define AvmVersion(major, minor, patch, tag) \
    AvmVersion_ctor(major, minor, patch, tag)

AVMAPI AvmVersion AvmVersion_ctor(uint32_t major, uint32_t minor,
                                  uint32_t patch, char8_t tag);
AVMAPI bool AvmVersionIsCompatible(AvmVersion self, AvmVersion other);
AVMAPI uint32_t AvmVersionGetMajor(AvmVersion self);
AVMAPI uint32_t AvmVersionGetMinor(AvmVersion self);
AVMAPI uint32_t AvmVersionGetPatch(AvmVersion self);
AVMAPI char8_t AvmVersionGetTag(AvmVersion self);

#endif  // AVIUM_CORE_H
