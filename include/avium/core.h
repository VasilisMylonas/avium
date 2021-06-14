/**
 * @file avium/core.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Avium core runtime utilities.
 * @version 0.3.5
 * @date 2021-05-28
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

#include "avium/config.h"

#include <assert.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @defgroup AvmCoreTypes Avium core types.
 *
 * @{
 */

typedef AVM_LONG_TYPE _long;          ///< Signed 64-bit integer type.
typedef unsigned AVM_LONG_TYPE ulong; ///< Unsigned 64-bit integer type.
typedef unsigned int uint;            ///< Unsigned 32-bit integer type.
typedef unsigned short ushort;        ///< Unsigned 16-bit integer type.
typedef unsigned char byte;           ///< Unsigned 8-bit integer type.
typedef void* object;                 ///< An unknown object type.
typedef void (*AvmCallback)(void);    ///< An unknown function type.
typedef const char* str;              ///< Primitive read-only string.
#define weakptr(T) T*                 ///< A weak pointer to a type T.
typedef struct AvmType AvmType;
typedef struct AvmEnum AvmEnum;
typedef struct AvmString AvmString;
typedef struct AvmMember AvmMember;
typedef struct AvmClass AvmClass;
typedef struct AvmInterface AvmInterface;
typedef struct AvmFunction AvmFunction;

/// @}

/**
 * @defgroup AvmDeclMacros Declaration macros.
 *
 * @{
 */

#define AVM_CLASS(T, B, ...)                                                   \
    typedef struct T T;                                                        \
    extern const AvmClass _AVM_METADATA_BLOCK_NAME(T);                         \
    struct T                                                                   \
    {                                                                          \
        union {                                                                \
            const AvmClass* __type;                                            \
            B __base;                                                          \
        };                                                                     \
        struct __VA_ARGS__;                                                    \
    }

#define AVM_INTERFACE(T, ...)                                                  \
    enum __VA_ARGS__;                                                          \
    AVM_CLASS(T, object, {                                                     \
        object Value;                                                          \
        const AvmCallback* Methods;                                            \
    })

/**
 * @brief Concatenates two identifiers
 *
 * @param a The first identifier.
 * @param b The second identifier.
 */
#define AVM_CONCAT(a, b) AVM_CONCAT_(a, b)

/**
 * @brief Creates a unique identifier based on a provided name.
 *
 * @param name The name of the identifier.
 */
#define AVM_UNIQUE(name) AVM_CONCAT(name, __LINE__)

// TODO: Remove
#define AvmInvalid ((uint)-1)
#define Main       AvmMain

/// @}

#ifdef AVM_DISABLE_PRECONDITIONS
#define pre if (false)
#else
#define pre if (true)
#endif

#ifdef AVM_DISABLE_POSTCONDITIONS
#define post if (false)
#else
#define post if (true)
#endif

/**
 * @defgroup AvmObjectFunctions Universal object instance functions.
 *
 * These functions provide the basic facilities for the rest of the Avium
 * type system.
 *
 * @{
 */

enum
{
    AvmEntryFinalize,
    AvmEntryToString,
};

/**
 * @brief Allocates an object on the heap.
 *
 * The object is zero-initialized, with proper typeinfo and a lazy mutex
 * lock.
 *
 * This function is not overridable.
 *
 * @pre type != NULL.
 * @post return != NULL.
 *
 * @param type The type of the object to allocate.
 * @return The object.
 */
AVMAPI object AvmObjectNew(const AvmClass* type) AVM_NONNULL(1);

/**
 * @brief Gets information about the type of an object.
 *
 * You should use this function to get the type of an object. This
 * assumes that the .__type field is correctly initialized by the object
 * constructor.
 *
 * This function returns the actual runtime type of an object bypassing
 * any base types. That is, for an object of type T where T inherits
 * from another type B, this function will always return typeid(T)
 * regardless if the object is passed as a T* or a B*.
 *
 * To check if an object is an instance of a type (possibly derived) use
 * the instanceof macro.
 *
 * To handle the returning AvmClass instance, you may need to include
 * typeinfo.h
 *
 * This function is not overridable.
 *
 * @pre self != NULL.
 * @post return != NULL.
 *
 * @param self The object instance.
 * @return The type information of the object.
 */
AVMAPI const AvmClass* AvmObjectGetType(object self) AVM_NONNULL(1) AVM_PURE;

/**
 * @brief Disables the finalizer of an object.
 *
 * When called makes the runtime ignore finalization of the object. This may be
 * useful if writing functions that deterministically finalize an object.
 *
 * This function is not overridable.
 *
 * @pre self != NULL.
 * @pre AvmRuntimeIsHeapObject(self).
 *
 * @param self The object instance.
 */
AVMAPI void AvmObjectSurpressFinalizer(object self) AVM_NONNULL(1);

/**
 * @brief Visits a member in an object.
 *
 * This function is generally unsafe, error-prone and should be used carefully.
 *
 * This function is not overridable.
 *
 * @pre self != NULL.
 * @pre member != NULL.
 *
 * @param self The object instance.
 * @param member The member to visit.
 * @return A pointer to the data.
 */
AVMAPI void* AvmObjectVisit(object self, const AvmMember* member)
    AVM_NONNULL(1, 2) AVM_PURE;

AVMAPI void AvmObjectLock(object self) AVM_NONNULL(1);
AVMAPI void AvmObjectUnlock(object self) AVM_NONNULL(1);

#define lock(o)                                                                \
    AvmObjectLock(o);                                                          \
    for (uint AVM_UNIQUE(__avmMC) = 0; AVM_UNIQUE(__avmMC) < 1;                \
         AVM_UNIQUE(__avmMC)++, AvmObjectUnlock(o))

/**
 * @brief Finalizes an object.
 *
 * This function calls AvmObjectSurpressFinalizer to make sure that the
 * finalizer is not called more than once.
 *
 * This function is overridable by the AvmEntryFinalize entry in the VFT.
 *
 * Overriding should be done when an object needs to release non-memory
 * resources.
 *
 * @pre self != NULL.
 * @pre AvmRuntimeIsHeapObject(self).
 *
 * @param self The object instance.
 */
AVMAPI void AvmObjectFinalize(object self) AVM_NONNULL(1);

/**
 * @brief Creates a string representation of an object.
 *
 * This function creates a simple object-identifying string. Overriding this
 * function to provide a more helpful representation may be useful.
 *
 * This function is overridable by the AvmEntryToString entry in the VFT.
 *
 * @pre self != NULL.
 *
 * @param self The object instance.
 * @return The string representation of the object.
 */
AVMAPI AvmString AvmObjectToString(object self) AVM_NONNULL(1);

/// @}

/**
 * @defgroup AvmMiscTypes Miscellaneous value types.
 *
 * Types for things such as source code locations and program versions.
 *
 * @{
 */

/// Represents a location in source code.
AVM_CLASS(AvmLocation, object, {
    str File;    ///< The file name.
    uint Line;   ///< The line number.
    uint Column; ///< The column number.
});

/**
 * @brief Creates a new AvmLocation instance.
 *
 * @pre file != NULL
 *
 * @param file The file name.
 * @param line The line number.
 * @param column The column.
 *
 * @return The created instance.
 */
AVMAPI AvmLocation AvmLocationFrom(str file, uint line, uint column)
    AVM_NONNULL(1) AVM_PURE;

/// A type representing a software version in the format: N.N.N
AVM_CLASS(AvmVersion, object, {
    ushort Major; ///< The major version.
    ushort Minor; ///< The minor version.
    ushort Patch; ///< The patch version.
});

/**
 * @brief Creates a new AvmVersion instance.
 *
 * @param major The version major number (incremented at breaking changes).
 * @param minor The version minor number (incremented at non-breaking changes).
 * @param patch The version patch number (incremented at bug fixes).
 *
 * @return The created instance.
 */
AVMAPI AvmVersion AvmVersionFrom(ushort major,
                                 ushort minor,
                                 ushort patch) AVM_PURE;

/// An iterator over the program arguments.
AVM_CLASS(AvmArgs, object, {
    const uint Length; ///< The total number of arguments.
    str Current;       ///< The current argument.

    struct
    {
        uint position;
        str* argv;
    } _private;
});

/**
 * @brief Advances the position of an argument iterator.
 *
 * If iteration has reached the end, then the iterator is reset and false is
 * returned.
 *
 * This can be used like so:
 *
 * @code
 * AvmArgs args = AvmRuntimeGetArgs();
 *
 * while (AvmArgsNext(&args)) {
 *     AvmPrintf("%s\n", args.Current);
 * }
 * @endcode
 *
 * @pre self != NULL.
 *
 * @param self The AvmArgs instance.
 * @return true if the iterator was advanced, false if it was reset.
 */
AVMAPI bool AvmArgsNext(AvmArgs* self) AVM_NONNULL(1);

/// @}

/**
 * @defgroup AvmRuntimeFunctions Builtin runtime functions.
 *
 * These functions provide the core features of the Avium runtime.
 *
 * @{
 */

typedef void (*AvmEntryPoint)(void); ///< The entry point function.
typedef byte AvmExitCode;            ///< A process or thread exit code.

/**
 * @brief Initializes the Avium runtime and calls into the entry point.
 *
 * The entry point is executed in a runtime context which is destroyed upon
 * return. Most Avium functions cannot be called outside a runtime context.
 * Doing so is undefined and will probably result in a crash.
 *
 * When linking with the core library this function is called automatically and
 * user code execution begins at Main unless the AVM_INSERT_INIT_CODE option is
 * set to OFF.
 *
 * Calling this function from a runtime context returns EXIT_FAILURE
 * immediately.
 *
 * @pre argc > 0.
 * @pre argv != NULL.
 * @pre entry != NULL.
 *
 * @param argc The argc parameter from main.
 * @param argv The argv parameter from main.
 * @param entry The entry point.
 *
 * @return A status code such as EXIT_SUCCESS or EXIT_FAILURE.
 */
AVMAPI AvmExitCode AvmRuntimeInit(int argc, str argv[], AvmEntryPoint entry)
    AVM_NONNULL(2, 3);

/**
 * @brief Returns the name of the currently running program.
 *
 * @return The name of the currently running program.
 */
AVMAPI str AvmRuntimeGetProgramName(void) AVM_PURE;

/**
 * @brief Returns the current runtime version.
 *
 * @return The current runtime version.
 */
AVMAPI AvmVersion AvmRuntimeGetVersion(void) AVM_PURE;

/**
 * @brief Returns an iterator over the program arguments.
 *
 * @return The program arguments.
 */
AVMAPI AvmArgs AvmRuntimeGetArgs(void) AVM_PURE;

/**
 * @brief Determines whether an object resides on the heap.
 *
 * @pre o != NULL.
 *
 * @param o The object.
 * @return true if the object was found in the heap, otherwise false.
 */
AVMAPI bool AvmRuntimeIsHeapObject(object o) AVM_NONNULL(1);

/**
 * @brief Captures a stack backtrace, if available.
 *
 * @return The backtrace, or a symbolic string.
 */
AVMAPI AvmString AvmRuntimeGetBacktrace(void);

/**
 * @brief Throws an object.
 *
 * A better alternative to using this function is the throw macro.
 *
 * @pre value != NULL.
 *
 * if AVM_THROW_AUTO_CLONE is not defined:
 *
 * @pre AvmRuntimeIsHeapObject(value).
 *
 * @param value The object to throw.
 * @param location The location from which the object is thrown.
 * @return This function never returns.
 */
AVMAPI never AvmRuntimeThrow(object value, AvmLocation location) AVM_NONNULL(1);

/// @}

/**
 * @defgroup AvmAllocationFunctions Memory allocation functions.
 *
 * These functions provide access to the memory allocator.
 *
 * @{
 */

/**
 * @brief Allocates heap memory.
 *
 * @pre size != 0.
 *
 * @param size The size of the memory block in bytes.
 * @return The allocated memory.
 */
AVMAPI void* AvmAlloc(uint size, bool containsPointers);

/**
 * @brief Reallocates a heap memory block.
 *
 * @pre size != 0.
 *
 * @param memory The memory block to reallocate.
 * @param size The new size of the memory block in bytes.
 * @return The reallocated memory.
 */
AVMAPI void* AvmRealloc(void* memory, uint size) AVM_NONNULL(1);

/// @}

/**
 * @defgroup AvmCoreIO Fundamental IO functions.
 *
 * @{
 */

/**
 * @brief Reads formated output from stdin.
 *
 * @pre format != NULL.
 * @pre args != NULL.
 *
 * @param format The format string.
 * @param args The format string arguments.
 */
AVMAPI void AvmVScanf(str format, va_list args) AVM_NONNULL(1, 2);

/**
 * @brief Writes formated output to stdout.
 *
 * @pre format != NULL.
 * @pre args != NULL.
 *
 * @param format The format string.
 * @param args The format string arguments.
 */
AVMAPI void AvmVPrintf(str format, va_list args) AVM_NONNULL(1, 2);

/**
 * @brief Writes formated output to stderr.
 *
 * @pre format != NULL.
 * @pre args != NULL.
 *
 * @param format The format string.
 * @param args The format string arguments.
 */
AVMAPI void AvmVErrorf(str format, va_list args) AVM_NONNULL(1, 2);

/**
 * @brief Reads formated output from stdin.
 *
 * @pre format != NULL.
 *
 * @param format The format string.
 * @param ... The format string arguments.
 */
AVMAPI void AvmScanf(str format, ...) AVM_NONNULL(1);

/**
 * @brief Writes formated output to stdout.
 *
 * @pre format != NULL.
 *
 * @param format The format string.
 * @param ... The format string arguments.
 */
AVMAPI void AvmPrintf(str format, ...) AVM_NONNULL(1);

/**
 * @brief Writes formated output to stderr.
 *
 * @pre format != NULL.
 *
 * @param format The format string.
 * @param ... The format string arguments.
 */
AVMAPI void AvmErrorf(str format, ...) AVM_NONNULL(1);

/// @}

/**
 * @defgroup AvmErrorHandling Error handling utilities.
 *
 * @{
 */

/// A type representing an error. TODO: make to proper interface
typedef void AvmError;

/**
 * @brief Creates an AvmError with a message.
 *
 * @pre Parameter @p message must be not null.
 *
 * @param message The error message.
 * @return The created instance.
 */
AVMAPI AvmError* AvmErrorNew(str message) AVM_NONNULL(1);

/**
 * @brief Creates an AvmError from an OS code.
 *
 * If the code is 0 then a 'successful' error is created.
 *
 * @param code The error code.
 * @return The created instance.
 */
AVMAPI AvmError* AvmErrorFromOSCode(int code);

/// Begins a try block.
#define try                                                                    \
    AvmThrowContext AVM_UNIQUE(__avmThrownContext);                            \
    __AvmRuntimePushThrowContext(&AVM_UNIQUE(__avmThrownContext));             \
    for (uint __avmTLC = 0; __avmTLC < 2; __avmTLC++)                          \
        if (__avmTLC == 1)                                                     \
        {                                                                      \
            if (__AvmRuntimeGetThrowContext()->_thrownObject == NULL)          \
            {                                                                  \
                __AvmRuntimePopThrowContext();                                 \
                break;                                                         \
            }                                                                  \
            AvmRuntimeThrow(__AvmRuntimeGetThrowContext()->_thrownObject,      \
                            __AvmRuntimePopThrowContext()->_location);         \
        }                                                                      \
        else if (setjmp(__AvmRuntimeGetThrowContext()->_jumpBuffer) == 0)

// clang-format off

/// Begins a catch block for type T.
#define catch(T, name)                                                         \
    else if (instanceof(T, __AvmRuntimeGetThrowContext()->_thrownObject) &&    \
                 __AvmRuntimeGetThrowContext() != NULL &&                      \
                 (__avmTLC = 2) ==                                             \
                     2) for (object name =                                     \
                                 __AvmRuntimePopThrowContext()->_thrownObject; \
                             name != NULL;                                     \
                             name = NULL)

// clang-format on

/**
 * @brief Throws an object.
 *
 * The object must be heap-allocated.
 *
 * @pre Parameter @p value must be not null.
 *
 * @param value The object to throw.
 */
#define throw(value)                                                           \
    AvmRuntimeThrow(value, AvmLocationFrom(__FILE__, __LINE__, 0))

/// @}

/**
 * @defgroup AvmBoxing Object representation for primitive types.
 *
 * @{
 */

/// Object equivalent of signed integers.
AVM_CLASS(AvmInteger, object, { _long Value; });

/// Object equivalent of unsigned integers.
AVM_CLASS(AvmUnsigned, object, { ulong Value; });

/// Object equivalent of floating point numbers.
AVM_CLASS(AvmFloat, object, { double Value; });

/**
 * @brief Creates an AvmInteger from an primitive.
 *
 * @param value The signed integer value.
 * @return The created AvmInteger instance.
 */
AVMAPI AvmInteger AvmIntegerFrom(_long value) AVM_PURE;

/**
 * @brief Creates an AvmUnsigned from an primitive.
 *
 * @param value The unsigned integer value.
 * @return The created AvmUnsigned instance.
 */
AVMAPI AvmUnsigned AvmUnsignedFrom(ulong value) AVM_PURE;

/**
 * @brief Creates an AvmFloat from an primitive.
 *
 * @param value The floating point value.
 * @return The created AvmFloat instance.
 */
AVMAPI AvmFloat AvmFloatFrom(double value) AVM_PURE;

/// @}

AVM_INTERFACE(AvmCloneable,
              {
                  AvmCloneableClone,
              });

AVM_INTERFACE(AvmEquatable,
              {
                  AvmEquatableEquals,
              });

/**
 * @brief Clones an object, creating an exact copy.
 *
 * Creates an exact clone of an object by copying the object to heap memory.
 *
 * The object type implements the AvmCloneable interface. This interface should
 * be re-implemented for a class if it contains pointers to other objects.
 *
 * @pre self != NULL.
 *
 * @param self The AvmCloneable instance.
 * @return The cloned object.
 */
AVMAPI object AvmClone(const AvmCloneable* self) AVM_NONNULL(1);

/**
 * @brief Compares for equality.
 *
 * This function should compare 2 objects for value equality, that is both
 * objects in-memory representation must be exactly the same for this function
 * to return true.
 *
 * The object type implements the AvmCloneable interface. This interface should
 *be re-implemented for non-trivial types.
 *
 * @pre self != NULL.
 * @pre other != NULL.
 * @pre AvmObjectGetType(self->Value) == AvmObjectGetType(other).
 *
 * @param self The first object.
 * @param other The second object.
 *
 * @return true if the two objects are equal, otherwise false.
 */
AVMAPI bool AvmEquals(const AvmEquatable* self, object other) AVM_NONNULL(1, 2);

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

#ifndef DOXYGEN
#define AVM_CONCAT_(a, b) a##b

typedef enum
{
    AvmArgErrorMsg,
    AvmMemErrorMsg,
    AvmRangeErrorMsg,
    AvmMarshalErrorMsg,
    AvmMissingMemberErrorMsg,
    AvmMissingConstantErrorMsg,
    AvmMissingCallbackErrorMsg,
    AvmThreadCreationErrorMsg,
    AvmThreadJoinErrorMsg,
    AvmThreadDetachErrorMsg,
    AvmInvalidStackSizeErrorMsg,
    AvmMissingInterfaceErrorMsg,
} AvmResourceKey;

AVMAPI str AvmRuntimeGetResource(AvmResourceKey key);

/// Represents the context of a thrown object.
AVM_CLASS(AvmThrowContext, object, {
    AvmThrowContext* _prev;
    object _thrownObject;
    AvmLocation _location;
    jmp_buf _jumpBuffer;
});

AVMAPI void __AvmRuntimePushThrowContext(AvmThrowContext*);
AVMAPI AvmThrowContext* __AvmRuntimePopThrowContext(void);
AVMAPI AvmThrowContext* __AvmRuntimeGetThrowContext(void);
#endif // DOXYGEN

#endif // AVIUM_CORE_H
