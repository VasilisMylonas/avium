/**
 * @file avium/core.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Avium core runtime utilities.
 * @version 0.3
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

#ifndef AVIUM_CORE_H
#define AVIUM_CORE_H

#include "avium/types.h"

/// Refers to the base type in a function with a self parameter.
#define base (&self->_base)

#define AvmInvalid ((size_t)-1)

/// Represents an entry on the virtual function table.
typedef enum
{
    FnEntryDtor = 0,          ///< The destructor entry.
    FnEntryToString,          ///< The AvmObjectToString entry.
    FnEntryClone,             ///< The AvmObjectClone entry.
    FnEntryEquals,            ///< The AvmObjectEquals entry.
    FnEntryRead = 16,         ///< The AvmStreamRead entry.
    FnEntryWrite,             ///< The AvmStreamWrite entry.
    FnEntrySeek,              ///< The AvmStreamSeek entry.
    FnEntryFlush,             ///< The AvmStreamFlush entry.
    FnEntryGetPosition,       ///< The AvmStreamPosition entry.
    FnEntryGetBacktrace = 16, ///< The AvmErrorGetBacktrace entry.
    FnEntryGetSource,         ///< The AvmErrorGetSource entry.

    FnEntryGetLength = 12,
    FnEntryGetCapacity,
} AvmFnEntry;

/**
 * @brief Gets information about the type of an object.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The object instance.
 * @return The type information of the object.
 */
AVMAPI const AvmType *AvmObjectGetType(object self);

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

#if defined AVM_GNU && defined AVM_LINUX
#pragma weak AvmAlloc
#pragma weak AvmRealloc
#pragma weak AvmDealloc
#endif

AVMAPI void AvmRuntimeInit(int argc, str argv[]);
AVMAPI str AvmRuntimeGetProgramName(void);

/**
 * @brief Allocates heap memory.
 *
 * @param size The size of the memory block in bytes.
 * @return The allocated memory.
 */
AVMAPI void *AvmAlloc(size_t size);

/**
 * @brief Reallocates a heap memory block.
 *
 * @param memory The memory block to reallocate.
 * @param size The new size of the memory block in bytes.
 * @return The reallocated memory.
 */
AVMAPI box(void) AvmRealloc(box(void) memory, size_t size);

/**
 * @brief Deallocates heap memory.
 *
 * @param memory The memory block to deallocate.
 */
AVMAPI void AvmDealloc(box(void) memory);

/**
 * @brief Allocates an object on the heap, and initializes it with the provided
 * data.
 *
 * @param size The size of the object.
 * @param data The data to initialize the object with (can be NULL).
 * @return The allocated object.
 */
AVMAPI object AvmObjectAlloc(size_t size, object data);

/**
 * @brief Shortcut for initializing a heap object.
 *
 * @param T The type of the object.
 * @param ... The object initializer.
 */
#define heapalloc(T, ...) AvmObjectAlloc(sizeof(T), (T[1]){__VA_ARGS__})

/// Enables signal catching.
AVMAPI void AvmEnableExceptions(void);

/// Disables signal catching.
AVMAPI void AvmDisableExceptions(void);

/**
 * @brief Aborts execution, printing a message and location information.
 *
 * @param message The message to print.
 */
#define AvmPanic(message) AvmPanicEx(message, __func__, __FILE__, __LINE__)

/**
 * @brief Aborts execution, printing a message and location information.
 *
 * @param message The message to be printed.
 * @param function The function name.
 * @param file The file name.
 * @param line The line number.
 *
 * @return This function never returns.
 */
AVMAPI never AvmPanicEx(str message, str function, str file, uint line);

/**
 * @brief Copies memory from one block to another.
 *
 * This will copy length bytes from source to destination, but not more than
 * size.
 *
 * @pre Parameter @p source must be not null.
 * @pre Parameter @p destination must be not null.
 *
 * @param source The memory block to copy from.
 * @param length The length of the source buffer.
 * @param destination The memory block to copy to.
 * @param size The size of the destination buffer.
 */
AVMAPI void AvmMemCopy(byte *source, size_t length, byte *destination,
                       size_t size);

AVMAPI void AvmVScanf(str format, va_list args);
AVMAPI void AvmVPrintf(str format, va_list args);
AVMAPI void AvmVErrorf(str format, va_list args);

AVMAPI void AvmScanf(str format, ...);
AVMAPI void AvmPrintf(str format, ...);
AVMAPI void AvmErrorf(str format, ...);

#endif // AVIUM_CORE_H
