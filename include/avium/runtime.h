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

#ifdef AVM_MSVC
#    define typeof(T) decltype(T)
#else
#    ifdef __clang__
#        pragma GCC diagnostic push
#        pragma GCC diagnostic ignored "-Wkeyword-macro"
#    endif
#    define typeof(T) __typeof__(T)
#    ifdef __clang__
#        pragma GCC diagnostic pop
#    endif
#endif

/// Returns the base type of an object.
#define baseof(x) ((typeof((x)->_base)*)x)

/// Determines whether an object is of specific type.
#define istype(T, x) (typeid(T) == AvmObjectGetType(x))

// clang-format off
#define instanceof(T, x) \
    (istype(T, x) || AvmTypeInheritsFrom(AvmObjectGetType(x), typeid(T)))
// clang-format on

/// Returns a pointer to the type info of type T.
#define typeid(T) (&AVM_TI_NAME(T))

#if defined AVM_GNU && defined AVM_LINUX
#    pragma weak AvmAlloc
#    pragma weak AvmRealloc
#    pragma weak AvmDealloc
#endif

AVMAPI void AvmRuntimeInit(int argc, str argv[]);
AVMAPI str AvmRuntimeGetProgramName(void);

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
AVMAPI void AvmMemCopy(byte* source, size_t length, byte* destination,
                       size_t size);

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

/**
 * @brief Returns the specified VFT entry of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @param index The VFT entry.
 * @return The function pointer.
 */
AVMAPI AvmFunction AvmTypeGetFunction(const AvmType* self, size_t index);

AVMAPI const AvmType* AvmTypeGetBase(const AvmType* self);

AVMAPI bool AvmTypeInheritsFrom(const AvmType* self, const AvmType* baseType);

AVMAPI void AvmVScanf(str format, va_list args);
AVMAPI void AvmVPrintf(str format, va_list args);
AVMAPI void AvmVErrorf(str format, va_list args);

AVMAPI void AvmScanf(str format, ...);
AVMAPI void AvmPrintf(str format, ...);
AVMAPI void AvmErrorf(str format, ...);

AVMAPI void AvmRuntimeRunTests(str program);

#ifndef _AVM_BUILD
void __main();

int main(int argc, str argv[]) {
    AvmRuntimeInit(argc, argv);
    AvmRuntimeRunTests(argv[0]);
    __main();
    return 0;
}
#endif

#define main __main

#endif  // AVIUM_RUNTIME_H
