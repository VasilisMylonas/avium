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

/**
 * @brief Creates an Avium class type.
 *
 * @param T The name of the type.
 * @param B The base class of the type.
 * @param ... Member declaration in braces ({ ... })
 */
#define AVM_CLASS(T, B, ...)             \
    typedef struct T T;                  \
    extern const AvmType AVM_TI_NAME(T); \
    struct T                             \
    {                                    \
        union                            \
        {                                \
            const AvmType *_type;        \
            B _base;                     \
        };                               \
        struct __VA_ARGS__;              \
    }

/**
 * @brief Generates type info for a type.
 *
 * @param T The type for which to generate type info.
 * @param B The base type.
 * @param ... The type vtable enclosed in braces ({...})
 */
#define AVM_TYPE(T, B, ...)          \
    const AvmType AVM_TI_NAME(T) = { \
        ._type = typeid(AvmType),    \
        ._vptr = __VA_ARGS__,        \
        ._name = #T,                 \
        ._baseType = typeid(B),      \
        ._size = sizeof(T),          \
    }

/**
 * @brief Creates an Avium interface type.
 *
 * @param T The name of the type.
 */
#define AVM_INTERFACE(T)      \
    typedef struct T T;       \
    struct T                  \
    {                         \
        const AvmType *_type; \
    }

/// A type containing information about an object.
AVM_CLASS(AvmType, object, {
    str _name;
    size_t _size;
    const AvmType *_baseType;
    AvmFunction _vptr[AVM_VFT_SIZE];
});

/// A dynamic heap-allocated string.
AVM_CLASS(AvmString, object, {
    uint _capacity;
    uint _length;
    char *_buffer;
});

#define AvmInvalid ((size_t)-1)

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

/// Represents an entry on the virtual function table.
typedef enum
{
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
#define typeof(T) decltype(T)
#else
#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wkeyword-macro"
#endif
#define typeof(T) __typeof__(T)
#ifdef __clang__
#pragma GCC diagnostic pop
#endif
#endif

/// Returns the base type of an object.
#define baseof(x) ((typeof((x)->_base) *)x)

/// Determines whether an object is of specific type.
#define istype(T, x) (typeid(T) == AvmObjectGetType(x))

// clang-format off
#define instanceof(T, x) \
    (istype(T, x) || AvmTypeInheritsFrom(AvmObjectGetType(x), typeid(T)))
// clang-format on

/// Returns a pointer to the type info of type T.
#define typeid(T) (&AVM_TI_NAME(T))

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

/**
 * @brief Gets the name of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @return The type's name.
 */
AVMAPI str AvmTypeGetName(const AvmType *self);

/**
 * @brief Gets the size of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @return The type's size.
 */
AVMAPI size_t AvmTypeGetSize(const AvmType *self);

/**
 * @brief Returns the specified VFT entry of a type.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmType instance.
 * @param index The VFT entry.
 * @return The function pointer.
 */
AVMAPI AvmFunction AvmTypeGetFunction(const AvmType *self, size_t index);

AVMAPI const AvmType *AvmTypeGetBase(const AvmType *self);

AVMAPI bool AvmTypeInheritsFrom(const AvmType *self, const AvmType *baseType);

AVMAPI void AvmVScanf(str format, va_list args);
AVMAPI void AvmVPrintf(str format, va_list args);
AVMAPI void AvmVErrorf(str format, va_list args);

AVMAPI void AvmScanf(str format, ...);
AVMAPI void AvmPrintf(str format, ...);
AVMAPI void AvmErrorf(str format, ...);

// Ensure type size constraints.
static_assert_s(sizeof(_long) == AVM_LONG_SIZE);
static_assert_s(sizeof(ulong) == AVM_LONG_SIZE);
static_assert_s(sizeof(int) == AVM_INT_SIZE);
static_assert_s(sizeof(uint) == AVM_INT_SIZE);
static_assert_s(sizeof(short) == AVM_SHORT_SIZE);
static_assert_s(sizeof(ushort) == AVM_SHORT_SIZE);
static_assert_s(sizeof(char) == AVM_CHAR_SIZE);
static_assert_s(sizeof(byte) == AVM_BYTE_SIZE);
static_assert_s(sizeof(AvmString) == AVM_STRING_SIZE);

#endif // AVIUM_CORE_H
