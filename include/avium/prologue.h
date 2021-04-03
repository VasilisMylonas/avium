/**
 * @file avium/prologue.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief The Avium prologue.
 * @version 0.1
 * @date 2021-03-17
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef AVIUM_PROLOGUE_H
#define AVIUM_PROLOGUE_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include "avium/config.h"
#include "avium/exports.h"

/**
 * @brief Concatenates two identifiers.
 *
 * @param x The first identifier.
 * @param y The second identifier.
 */
#define AVM_CONCAT(x, y) AVM_CONCAT_(x, y)

/**
 * @brief Converts an identifier to a string.
 *
 * @param x The identifier to stringify.
 */
#define AVM_STRINGIFY(x) AVM_STRINGIFY_(x)

/**
 * @brief Aborts execution, printing a message and location information.
 *
 * @param message The message to print.
 */
#define AvmPanic(message) AvmPanicEx(message, __func__, __FILE__, __LINE__)

/**
 * @defgroup avm_core_types Core primitive types.
 *
 * @{
 */

/// Signed pointer-sized integer type.
typedef AVM_PTR_TYPE ptr;

/// Unsigned pointer-sized integer type.
typedef unsigned AVM_PTR_TYPE uptr;

/// Signed 64-bit integer type.
typedef AVM_LONG_TYPE _long;

/// Unsigned 64-bit integer type.
typedef unsigned AVM_LONG_TYPE ulong;

/// Unsigned 32-bit integer type.
typedef unsigned int uint;

/// Unsigned 16-bit integer type.
typedef unsigned short ushort;

/// Unsigned 8-bit integer type.
typedef unsigned char byte;

/// Read-only string type.
typedef const char* str;

/// An unknown object type.
typedef void* object;

/// An unknown function type.
typedef void (*AvmFunction)(void);

#ifdef AVM_MSVC
/// A type signifying that a function never returns.
#    define never __declspec(noreturn) void
#else
/// A type signifying that a function never returns.
#    define never _Noreturn void
#endif  // AVM_MSVC

/// @}

#define AVM_GET_TYPE(T)  AVM_GET_TYPE_(T)
#define AVM_TYPE(T, ...) AVM_TYPE_(T, __VA_ARGS__)

enum {
    FUNC_DTOR = 0,
    FUNC_GET_LENGTH,
    FUNC_GET_CAPACITY,
    FUNC_TO_STRING,
    FUNC_CLONE,
    FUNC_EQUALS,
    FUNC_READ,
    FUNC_WRITE,
    FUNC_READ_STRING,
    FUNC_WRITE_STRING,
};

#define AVM_CLASS(T, B, ...)      \
    typedef struct T T;           \
    struct T {                    \
        union {                   \
            const AvmType* _type; \
            B _base;              \
        };                        \
        struct __VA_ARGS__;       \
    }

/**
 * @brief Aborts execution, printing a message and location information.
 *
 * @param message The message to be printed.
 * @param function The function name.
 * @param file The file name.
 * @param line The line number.
 *
 * @return never This function never returns.
 */
AVMAPI never AvmPanicEx(str message, str function, str file, uint line);

AVMAPI void AvmEnableExceptions(void);
AVMAPI void AvmDisableExceptions(void);

/// Describes the type of the error that occurred.
typedef enum {
    /// No error occurred.
    ErrorKindNone = 0,

    /// An invalid argument was received by a function.
    ErrorKindArg,

    /// A provided index was out of range.
    ErrorKindRange,

    /// There was not enough memory to handle an operation.
    ErrorKindMem,

    /// A function call was invalid for the current state.
    ErrorKindInvalidOp,

    /// An IO error occurred.
    ErrorKindIO,

    /// An unknown system error occurred.
    ErrorKindSys,

    /// A required resource was unavailable.
    ErrorKindNotFound,
} AvmErrorKind;

#define AVM_GENERIC(name, T) name##_##T

#define AvmResult(T)    AVM_GENERIC(AvmResult, T)
#define AvmSuccess(T)   AVM_GENERIC(AvmSuccess, T)
#define AvmFailure(T)   AVM_GENERIC(AvmFailure, T)
#define AvmUnwrap(T)    AVM_GENERIC(AvmUnwrap, T)
#define AvmIsFailure(T) AVM_GENERIC(AvmIsFailure, T)

#define AvmOptional(T) AVM_GENERIC(AvmOptional, T)
#define AvmSome(T)     AVM_GENERIC(AvmSome, T)
#define AvmNone(T)     AVM_GENERIC(AvmNone, T)
#define AvmHasValue(T) AVM_GENERIC(AvmHasValue, T)
#define AvmGetValue(T) AVM_GENERIC(AvmGetValue, T)

#define AVM_RESULT_TYPE(T)                                                   \
    AVM_CLASS(AVM_GENERIC(AvmResult, T), object, {                           \
        AvmErrorKind _kind;                                                  \
        union {                                                              \
            T _value;                                                        \
            str _error;                                                      \
        };                                                                   \
    });                                                                      \
                                                                             \
    AVM_TYPE(AVM_GENERIC(AvmResult, T), {[FUNC_DTOR] = NULL});               \
                                                                             \
    static inline AvmResult(T) AvmSuccess(T)(T value) {                      \
        return (AvmResult(T)){                                               \
            ._type = AVM_GET_TYPE(AVM_GENERIC(AvmResult, T)),                \
            ._kind = ErrorKindNone,                                          \
            ._value = value,                                                 \
        };                                                                   \
    }                                                                        \
                                                                             \
    static inline AvmResult(T) AvmFailure(T)(AvmErrorKind kind, str error) { \
        return (AvmResult(T)){                                               \
            ._type = AVM_GET_TYPE(AVM_GENERIC(AvmResult, T)),                \
            ._kind = kind,                                                   \
            ._error = error,                                                 \
        };                                                                   \
    }                                                                        \
                                                                             \
    static inline T AvmUnwrap(T)(AvmResult(T) * self) {                      \
        if (self->_kind == ErrorKindNone) {                                  \
            return self->_value;                                             \
        }                                                                    \
                                                                             \
        AvmPanic("Tried to unwrap a result describing failure.");            \
    }                                                                        \
                                                                             \
    static inline bool AvmIsFailure(T)(AvmResult(T) * self) {                \
        return self->_kind != ErrorKindNone;                                 \
    }

#define AVM_OPTIONAL_TYPE(T)                                     \
    AVM_CLASS(AVM_GENERIC(AvmOptional, T), object, {             \
        T _value;                                                \
        bool _hasValue;                                          \
    });                                                          \
                                                                 \
    AVM_TYPE(AVM_GENERIC(AvmOptional, T), {[FUNC_DTOR] = NULL}); \
                                                                 \
    static inline AvmOptional(T) AvmSome(T)(T value) {           \
        return (AvmOptional(T)){                                 \
            ._type = AVM_GET_TYPE(AVM_GENERIC(AvmOptional, T)),  \
            ._hasValue = true,                                   \
            ._value = value,                                     \
        };                                                       \
    }                                                            \
                                                                 \
    static inline AvmOptional(T) AvmNone(T)(void) {              \
        return (AvmOptional(T)){                                 \
            ._type = AVM_GET_TYPE(AVM_GENERIC(AvmOptional, T)),  \
            ._hasValue = false,                                  \
        };                                                       \
    }                                                            \
                                                                 \
    static inline bool AvmHasValue(T)(AvmOptional(T) * self) {   \
        return self->_hasValue;                                  \
    }                                                            \
                                                                 \
    static inline T AvmGetValue(T)(AvmOptional(T) * self) {      \
        if (self->_hasValue) {                                   \
            return self->_value;                                 \
        }                                                        \
                                                                 \
        AvmPanic("Tried to unwrap an empty optional.");          \
    }

/**
 * @brief Creates a scope in which an object will be available and be destroyed
 *        at scope exit.
 *
 * @param T The type of the object.
 * @param name The name of the variable.
 * @param init The object initializer.
 */
#define defer(T, name, init) \
    for (T name = init; name != NULL; AvmObjectDestroy(name), name = NULL)

/// A type containing information about an object.
typedef struct AvmType AvmType;

struct AvmType {
    const AvmFunction* _vptr;
    str _name;
    size_t _size;
};

/**
 * @brief Gets the name of a type.
 *
 * @param self The type.
 * @return str The type's name.
 */
AVMAPI str AvmTypeGetName(const AvmType* self);

/**
 * @brief Gets the size of a type.
 *
 * @param self The type.
 * @return size_t The type's size.
 */
AVMAPI size_t AvmTypeGetSize(const AvmType* self);

/// A dynamic string.
AVM_CLASS(AvmString, object, {
    size_t _capacity;
    size_t _length;
    char* _buffer;
});

/**
 * @brief Gets information about the type of an object.
 *
 * @param self The object.
 * @return AvmType The type information of the object.
 */
AVMAPI const AvmType* AvmObjectGetType(object self);

/**
 * @brief Compares two objects for equality.
 *
 * This function tries to use the FUNC_EQ virtual function entry to compare
 * for equality. If no such virtual function is available then memcmp is used.
 *
 * @param self The first object.
 * @param other The second object.
 *
 * @return true if the two objects are equal, otherwise false.
 */
AVMAPI bool AvmObjectEquals(object self, object other);

/**
 * @brief Destroys an object and deallocates its memory.
 *
 * This function tries to use the FUNC_DTOR virtual function entry to destroy
 * the object. If no such virtual function then this function does nothing.
 *
 * @param self The object.
 */
AVMAPI void AvmObjectDestroy(object self);

/**
 * @brief Clones an object, creating an exact copy.
 *
 * This function tries to use the FUNC_CLONE virtual function entry to clone
 * the object. If no such virtual function is available then a combination of
 * malloc and memcpy is used.
 *
 * @param self The object.
 * @return object The cloned object.
 */
AVMAPI object AvmObjectClone(object self);

/**
 * @brief Creates a string representation of an object.
 *
 * This function tries to use the FUNC_TO_STRING virtual function entry to
 * create a string representation of the object. If no such virtual function
 * is available then this function traps.
 *
 * @param self The object.
 * @return AvmString The string representation of the object.
 *
 * @see AvmVirtualFunctionTrap
 */
AVMAPI AvmString AvmObjectToString(object self);

AVMAPI void AvmObjectCopy(object self, size_t size, byte buffer[]);

/**
 * @brief The trap function called when a virtual function is not implemented.
 *
 * @return never This function never returns.
 */
AVMAPI never AvmVirtualFunctionTrap(void);

/**
 * @brief Copies memory from one block to another.
 *
 * This will copy length bytes from source to destination, but not more than
 * size.
 *
 * @param source The memory block to copy from.
 * @param length The length of the source buffer.
 * @param destination The memory block to copy to.
 * @param size The size of the destination buffer.
 */
AVMAPI void AvmMemCopy(byte* source, size_t length, byte* destination,
                       size_t size);

// Ensure correct type sizes.
static_assert_s(sizeof(ptr) == sizeof(void*));
static_assert_s(sizeof(uptr) == sizeof(void*));
static_assert_s(sizeof(_long) == AVM_LONG_SIZE);
static_assert_s(sizeof(ulong) == AVM_LONG_SIZE);
static_assert_s(sizeof(int) == AVM_INT_SIZE);
static_assert_s(sizeof(uint) == AVM_INT_SIZE);
static_assert_s(sizeof(short) == AVM_SHORT_SIZE);
static_assert_s(sizeof(ushort) == AVM_SHORT_SIZE);
static_assert_s(sizeof(char) == AVM_CHAR_SIZE);
static_assert_s(sizeof(byte) == AVM_BYTE_SIZE);
static_assert_s(sizeof(AvmString) == AVM_STRING_SIZE);

#define AVM_CONCAT_(x, y) x##y
#define AVM_STRINGIFY_(x) #x

#define AVM_TYPE_(T, ...)                                \
    static const AvmType _##T##Type = {                  \
        ._vptr = (AvmFunction[AVM_VFT_SIZE])__VA_ARGS__, \
        ._name = #T,                                     \
        ._size = sizeof(T),                              \
    }

#define AVM_GET_TYPE_(T) &_##T##Type

AVM_RESULT_TYPE(char)
AVM_RESULT_TYPE(byte)
AVM_RESULT_TYPE(short)
AVM_RESULT_TYPE(ushort)
AVM_RESULT_TYPE(int)
AVM_RESULT_TYPE(uint)
AVM_RESULT_TYPE(_long)
AVM_RESULT_TYPE(ulong)
AVM_RESULT_TYPE(ptr)
AVM_RESULT_TYPE(uptr)
AVM_RESULT_TYPE(bool)
AVM_RESULT_TYPE(str)
AVM_RESULT_TYPE(object)
AVM_RESULT_TYPE(size_t)
AVM_RESULT_TYPE(AvmString)
AVM_RESULT_TYPE(AvmFunction)

AVM_OPTIONAL_TYPE(char)
AVM_OPTIONAL_TYPE(byte)
AVM_OPTIONAL_TYPE(short)
AVM_OPTIONAL_TYPE(ushort)
AVM_OPTIONAL_TYPE(int)
AVM_OPTIONAL_TYPE(uint)
AVM_OPTIONAL_TYPE(_long)
AVM_OPTIONAL_TYPE(ulong)
AVM_OPTIONAL_TYPE(ptr)
AVM_OPTIONAL_TYPE(uptr)
AVM_OPTIONAL_TYPE(bool)
AVM_OPTIONAL_TYPE(str)
AVM_OPTIONAL_TYPE(object)
AVM_OPTIONAL_TYPE(size_t)
AVM_OPTIONAL_TYPE(AvmString)
AVM_OPTIONAL_TYPE(AvmFunction)

#endif  // AVIUM_PROLOGUE_H
