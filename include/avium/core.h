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

/// A type representing a software version in the format: N.N.N
AVM_CLASS(AvmVersion, object, {
    ushort Major; ///< The major version.
    ushort Minor; ///< The minor version.
    ushort Patch; ///< The patch version.
});

/// Refers to the base type in a function with a self parameter.
#define base (&self->_base)

// TODO
#define AvmInvalid ((uint)-1)

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
 * @brief Return an AvmVersion indicating the current runtime version.
 *
 * @return The current runtime version.
 */
AVMAPI AvmVersion AvmRuntimeGetVersion(void);

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

AVM_CLASS(AvmEnv, object, {
    bool _isInitialized;
    uint _flags;
    str _name;
    str _libPath;
    AvmVersion _version;
    str _programName;
    str* _args;
});

/**
 * @brief Initializes the Avium runtime.
 *
 * Many Avium functions cannot be called without initializing the runtime
 * first.
 *
 * @param argc The argc parameter from main.
 * @param argv The argv parameter from main.
 * @return The runtime environment.
 */
AVMAPI weakptr(AvmEnv) AvmRuntimeInit(int argc, str argv[]);

/// Returns the name of the currently running program.
AVMAPI str AvmRuntimeGetProgramName(void);

/// Enables signal catching.
AVMAPI void AvmRuntimeEnableExceptions(void);

/// Disables signal catching.
AVMAPI void AvmRuntimeDisableExceptions(void);

/**
 * @brief Captures a backtrace, if available.
 *
 * @return The backtrace, or a symbolic string.
 */
AVMAPI AvmString AvmRuntimeGetBacktrace(void);

/**
 * @brief Allocates heap memory.
 *
 * @param size The size of the memory block in bytes.
 * @return The allocated memory.
 */
AVMAPI void* AvmAlloc(size_t size);

/**
 * @brief Reallocates a heap memory block.
 *
 * @param memory The memory block to reallocate.
 * @param size The new size of the memory block in bytes.
 * @return The reallocated memory.
 */
AVMAPI void* AvmRealloc(void* memory, size_t size);

/**
 * @brief Deallocates heap memory.
 *
 * @param memory The memory block to deallocate.
 */
AVMAPI void AvmDealloc(void* memory);

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

/**
 * @brief Creates a new AvmVersion instance.
 *
 * @param major The version major number (incremented at breaking changes).
 * @param minor The version minor number (incremented at non-breaking changes).
 * @param patch The version patch number (incremented at bug fixes).
 *
 * @return The created instance.
 */
AVMAPI AvmVersion AvmVersionFrom(ushort major, ushort minor, ushort patch);

#ifndef DOXYGEN
static_assert_s(sizeof(AvmVersion) == AVM_VERSION_SIZE);
#endif // DOXYGEN

#endif // AVIUM_CORE_H
