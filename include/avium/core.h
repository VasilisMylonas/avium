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

#if defined AVM_GNU && defined AVM_LINUX
#pragma weak AvmAlloc
#pragma weak AvmRealloc
#pragma weak AvmDealloc
#endif

/// Refers to the base type in a function with a self parameter.
#define base (&self->_base)

// TODO
#define AvmInvalid ((uint)-1)

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

/**
 * @brief Initializes the Avium runtime.
 *
 * Many Avium functions cannot be called without initializing the runtime
 * first.
 *
 * @param argc The argc parameter from main.
 * @param argv The argv parameter from main.
 */
AVMAPI void AvmRuntimeInit(int argc, str argv[]);

/// Returns the name of the currently running program.
AVMAPI str AvmRuntimeGetProgramName(void);

/// Enables signal catching.
AVMAPI void AvmRuntimeEnableExceptions(void);

/// Disables signal catching.
AVMAPI void AvmRuntimeDisableExceptions(void);

/**
 * @brief Allocates heap memory.
 *
 * @param size The size of the memory block in bytes.
 * @return The allocated memory.
 */
AVMAPI box(void) AvmAlloc(size_t size);

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
 * @brief Copies an object to a memory block.
 *
 * @pre Parameter @p o must be not null.
 * @pre Parameter @p destination must be not null.
 *
 * @param o The object to copy.
 * @param size The size of the destination buffer.
 * @param destination The memory block to copy to.
 */
AVMAPI void AvmCopy(object o, size_t size, byte* destination);

AVMAPI void AvmVScanf(str format, va_list args);
AVMAPI void AvmVPrintf(str format, va_list args);
AVMAPI void AvmVErrorf(str format, va_list args);

AVMAPI void AvmScanf(str format, ...);
AVMAPI void AvmPrintf(str format, ...);
AVMAPI void AvmErrorf(str format, ...);

#endif // AVIUM_CORE_H
