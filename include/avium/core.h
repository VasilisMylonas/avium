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
#include "avium/exports.h"

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
typedef struct AvmFunction AvmFunction;

/// @}

/**
 * @brief Creates an Avium class type.
 *
 * @param T The name of the type.
 * @param B The base class of the type.
 * @param ... Member declaration in braces ({ ... })
 */
#define AVM_CLASS(T, B, ...)                                                   \
    typedef struct T T;                                                        \
    extern const AvmType AVM_TI_NAME(T);                                       \
    struct T                                                                   \
    {                                                                          \
        union {                                                                \
            const AvmType* _type;                                              \
            B _base;                                                           \
        };                                                                     \
        struct __VA_ARGS__;                                                    \
    }

/**
 * @brief Creates an Avium interface type.
 *
 * @param T The name of the type.
 */
#define AVM_INTERFACE(T) typedef void T

/**
 * @brief Creates an Avium enum type.
 *
 * @param T The name of the enum.
 * @param ... The enum constants enclosed in braces ({...}).
 */
#define AVM_ENUM(T, ...)                                                       \
    typedef enum T __VA_ARGS__ T;                                              \
    extern const AvmEnum AVM_TI_NAME(T)

#define AVM_CONCAT_(a, b) a##b
#define AVM_CONCAT(a, b)  AVM_CONCAT_(a, b)
#define AVM_UNIQUE(name)  AVM_CONCAT(name, __LINE__)
#define AVM_VA_ARGS(T, ...)                                                    \
    (sizeof((T[]){__VA_ARGS__}) / sizeof(T)), (T[])                            \
    {                                                                          \
        __VA_ARGS__                                                            \
    }

#define Main AvmMain

// TODO
#define AvmInvalid ((uint)-1)

#if defined AVM_GNU && defined AVM_LINUX
#pragma weak AvmAlloc
#pragma weak AvmRealloc
#endif

/**
 * @defgroup AvmObjectFunctions Universal object instance functions.
 *
 * These functions provide the basic facilities for the rest of the Avium
 * type system.
 *
 * @{
 */

/**
 * @brief Allocates an object on the heap.
 *
 * TODO
 *
 * This function is not overridable.
 *
 * @pre Parameter @p type must be not null.
 *
 * @param type The type of the object to allocate.
 * @return The object.
 */
AVMAPI object AvmObjectNew(const AvmType* type);

/**
 * @brief Gets information about the type of an object.
 *
 * You should use this function to get the type of an object. This
 * assumes that the ._type field is correctly initialized by the object
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
 * To handle the returning AvmType instance, you may need to include
 * typeinfo.h
 *
 * This function is not overridable.
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
 * This function compares 2 objects for value equality, that is both objects
 * in-memory representation must be exactly the same for this function to return
 * true.
 *
 * Overriding this function may be especially desirable for non-trivial types.
 *
 * This function is overridable by the FnEntryEquals entry in the VFT.
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
 * @brief Finalizes an object.
 *
 * This function calls AvmObjectSurpressFinalizer to make sure that the
 * finalizer is not called more than once.
 *
 * This function is overridable by the FnEntryFinalize entry in the VFT.
 *
 * Overriding should be done when an object needs to release non-memory
 * resources.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The object instance.
 */
AVMAPI void AvmObjectFinalize(object self);

/**
 * @brief Disables the finalizer of an object.
 *
 * When called makes the runtime ignore finalization of the object. This may be
 * useful if writing functions that deterministically finalizes an object.
 *
 * This function is not overridable.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The object instance.
 */
AVMAPI void AvmObjectSurpressFinalizer(object self);

/**
 * @brief Clones an object, creating an exact copy.
 *
 * Creates an exact clone of an objects such as that AvmObjectEquals returns
 * true by copying the object to heap memory.
 *
 * This function should be overriden if an object contains pointers.
 *
 * This function is overridable by the FnEntryClone entry in the VFT.
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
 * This function creates a simple object-identifying string. Overriding this
 * function to provided a more helpful representation may be useful.
 *
 * This function is overridable by the FnEntryToString entry in the VFT.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The object instance.
 * @return The string representation of the object.
 */
AVMAPI AvmString AvmObjectToString(object self);

/**
 * @brief Visits a member in an object.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p member must be not null.
 *
 * @param self The object instance.
 * @param member The member to visit.
 * @return A pointer to the data.
 */
AVMAPI void* AvmObjectVisit(object self, const AvmMember* member);

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
 * @param file The file name.
 * @param line The line number.
 * @param column The column.
 *
 * @return The created instance.
 */
AVMAPI AvmLocation AvmLocationFrom(str file, uint line, uint column);

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
AVMAPI AvmVersion AvmVersionFrom(ushort major, ushort minor, ushort patch);

/// @}

/**
 * @defgroup AvmRuntimeFunctions Builtin runtime functions.
 *
 * These functions provide the core features of the Avium runtime.
 *
 * @{
 */

/// Represents the context of a thrown object.
AVM_CLASS(AvmThrowContext, object, {
    AvmThrowContext* _prev;
    object _thrownObject;
    AvmLocation _location;
    jmp_buf _jumpBuffer;
});

/// Holds the state for the Avium runtime.
AVM_CLASS(AvmRuntime, object, {
    uint _argc;
    str* _argv;
    str _name;
    AvmVersion _version;
});

/// The entry point function.
typedef void (*AvmEntryPoint)(void);

typedef byte AvmExitCode;

/**
 * @brief Initializes the Avium runtime and calls into the entry point.
 *
 * Many Avium functions cannot be called without initializing the runtime
 * first.
 *
 * When using the whole framework, this function is called automatically.
 *
 * @param argc The argc parameter from main.
 * @param argv The argv parameter from main.
 * @param entry The entry point.
 *
 * @return A status code such as EXIT_SUCCESS or EXIT_FAILURE.
 */
AVMAPI AvmExitCode AvmRuntimeInit(int argc, str argv[], AvmEntryPoint entry);

/**
 * @brief Returns the name of the currently running program.
 *
 * @return The name of the currently running program.
 */
AVMAPI str AvmRuntimeGetProgramName(void);

/**
 * @brief Returns the current runtime version.
 *
 * @return The current runtime version.
 */
AVMAPI AvmVersion AvmRuntimeGetVersion(void);

/**
 * @brief Returns the number of argument that the program received.
 *
 * @return The number of arguments.
 */
AVMAPI uint AvmRuntimeGetArgCount(void);

/**
 * @brief Returns a pointer to the program arguments.
 *
 * @return The program arguments.
 */
AVMAPI str* AvmRuntimeGetArgs(void);

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
 * @pre Parameter @p value must be not null.
 *
 * @param value The object to throw.
 * @param location The location from which the object is thrown.
 * @return This function never returns.
 */
AVMAPI never AvmRuntimeThrow(object value, AvmLocation location);

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

/// @}

/**
 * @defgroup AvmCoreIO Fundamental IO functions.
 *
 * @{
 */

// TODO: Document this finally.

AVMAPI void AvmVScanf(str format, va_list args);
AVMAPI void AvmVPrintf(str format, va_list args);
AVMAPI void AvmVErrorf(str format, va_list args);

AVMAPI void AvmScanf(str format, ...);
AVMAPI void AvmPrintf(str format, ...);
AVMAPI void AvmErrorf(str format, ...);

/// @}

/**
 * @defgroup AvmErrorHandling Error handling utilities.
 *
 * @{
 */

/// A type representing an error.
AVM_INTERFACE(AvmError);

/**
 * @brief Creates an AvmError with a message.
 *
 * @pre Parameter @p message must be not null.
 *
 * @param message The error message.
 * @return The created instance.
 */
AVMAPI AvmError* AvmErrorNew(str message);

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

/**
 * @brief Creates an array from a va_list.
 *
 * @pre Parameter @p N must be not zero.
 * @pre Parameter @p args must be not null.
 *
 * @param T The type of the array elements.
 * @param N The length of the array.
 * @param args The va_list.
 *
 * @return The created array.
 */
#define va_array(T, N, args) (T*)__AvmRuntimeMarshalVaList(args, sizeof(T), N)

AVM_CLASS(AvmBox, object, {
    union {
        double AsFloat;
        ulong AsUint;
        _long AsInt;
        str AsStr;
    };
});

#define box(x)                                                                 \
    _Generic((x), char                                                         \
             : AvmRuntimeBoxInt, short                                         \
             : AvmRuntimeBoxInt, int                                           \
             : AvmRuntimeBoxInt, _long                                         \
             : AvmRuntimeBoxInt, byte                                          \
             : AvmRuntimeBoxUint, ushort                                       \
             : AvmRuntimeBoxUint, uint                                         \
             : AvmRuntimeBoxUint, ulong                                        \
             : AvmRuntimeBoxUint, float                                        \
             : AvmRuntimeBoxFloat, double                                      \
             : AvmRuntimeBoxFloat, str                                         \
             : AvmRuntimeBoxStr, char*                                         \
             : AvmRuntimeBoxStr)(x)

AVMAPI AvmBox AvmRuntimeBoxInt(_long value);
AVMAPI AvmBox AvmRuntimeBoxFloat(double value);
AVMAPI AvmBox AvmRuntimeBoxUint(ulong value);
AVMAPI AvmBox AvmRuntimeBoxStr(str value);

#ifndef DOXYGEN
AVMAPI void __AvmRuntimePushThrowContext(AvmThrowContext*);
AVMAPI AvmThrowContext* __AvmRuntimePopThrowContext(void);
AVMAPI AvmThrowContext* __AvmRuntimeGetThrowContext(void);
AVMAPI void* __AvmRuntimeMarshalVaList(va_list, uint, uint);
AVMAPI AvmExitCode __AvmRuntimeThreadInit(void*);
#endif // DOXYGEN

#endif // AVIUM_CORE_H
