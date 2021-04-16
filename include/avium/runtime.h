/**
 * @file avium/runtime.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Avium runtime utilities.
 * @version 0.2
 * @date 2021-04-14
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

#ifndef AVIUM_RUNTIME_H
#define AVIUM_RUNTIME_H

#include "avium/types.h"

/// Represents an entry on the virtual function table.
typedef enum {
    /// The VFT destructor entry.
    FnEntryDtor = 0,

    /// The VFT AvmObjectToString entry.
    FnEntryToString,

    /// The VFT AvmObjectClone entry.
    FnEntryClone,

    /// The VFT AvmObjectEquals entry.
    FnEntryEquals,

    // AvmStream
    FnEntryRead = 16,
    FnEntryWrite,
    FnEntrySeek,
    FnEntryFlush,
    FnEntryGetPosition,

    FnEntryGetLength = 12,
    FnEntryGetCapacity,

    /// The VFT AvmErrorGetBacktrace entry.
    FnEntryGetBacktrace = 16,

    /// The VFT AvmErrorGetSource entry.
    FnEntryGetSource,
} AvmFnEntry;

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
AVMAPI void AvmMemCopy(byte* source, size_t length, byte* destination,
                       size_t size);

/// Returns a pointer to the type info of type T.
#define AVM_GET_TYPE(T) AVM_GET_TYPE_(T)

/**
 * @brief Generates type info for a type.
 *
 * @param T The type for which to generate type info.
 * @param ... The type vtable enclosed in braces ({...})
 */
#define AVM_TYPE(T, ...) AVM_TYPE_(T, __VA_ARGS__)

/**
 * @brief Gets the name of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @return The type's name.
 */
AVMAPI str AvmTypeGetName(const AvmType* self);

/**
 * @brief Gets the size of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @return The type's size.
 */
AVMAPI size_t AvmTypeGetSize(const AvmType* self);

#ifndef DOXYGEN
#    define AVM_TYPE_(T, ...)         \
        static AvmType _##T##Type = { \
            ._vptr = __VA_ARGS__,     \
            ._name = #T,              \
            ._size = sizeof(T),       \
        }

#    define AVM_GET_TYPE_(T) &_##T##Type

struct AvmType {
    const str _name;
    const size_t _size;
    AvmFunction _vptr[AVM_VFT_SIZE];
};
#endif  // DOXYGEN

#endif  // AVIUM_RUNTIME_H
