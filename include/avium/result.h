/**
 * @file avium/result.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Result and error types.
 * @version 0.2
 * @date 2021-04-10
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

#ifndef AVIUM_RESULT_H
#define AVIUM_RESULT_H

#include "avium/runtime.h"

/// A type representing an error.
AVM_INTERFACE(AvmError);

/// Describes the type of the error that occurred.
typedef enum {
    /// Received an invalid argument.
    ErrorKindArg,

    /// The provided index was out of range.
    ErrorKindRange,

    /// There was not enough memory to handle the request.
    ErrorKindMem,

    /// The call was invalid for the current state.
    ErrorKindInvalidOp,

    /// An IO error occurred.
    ErrorKindIO,

    /// An unknown system error occurred.
    ErrorKindSys,

    /// A required resource was unavailable.
    ErrorKindNotFound,

    /// Could not perform the read operation.
    ErrorKindRead,

    /// Could not perform the write operation.
    ErrorKindWrite,
} AvmErrorKind;

/**
 * @brief Returns the last error that occurred.
 *
 * The error is created based on the errno value and may create a 'successful'
 * error.
 *
 * @return A type that implements AvmError.
 */
AVMAPI AvmError* AvmErrorGetLast(void);

/**
 * @brief Creates an AvmError from an os code.
 *
 * If the code is 0 then a 'successful' error is created.
 *
 * @param code The error code.
 * @return The created instance.
 */
AVMAPI AvmError* AvmErrorFromOSCode(int code);

/**
 * @brief Creates an AvmError of a specific kind.
 *
 * @param kind The error kind.
 * @return The created instance.
 */
AVMAPI AvmError* AvmErrorOfKind(AvmErrorKind kind);

/**
 * @brief Returns the AvmError responsible for this error.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmError instance.
 * @return The source of the error.
 */
AVMAPI AvmError* AvmErrorGetSource(AvmError* self);

/**
 * @brief Gets a backtrace of the stack during the creation of an error.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmError instance.
 * @return The backtrace.
 */
AVMAPI AvmString AvmErrorGetBacktrace(AvmError* self);

/**
 * @brief A type which can contain either an AvmError or a T.
 *
 * @param T The type.
 */
#define AvmResult(T) AVM_GENERIC(AvmResult, T)

#ifdef DOXYGEN
/**
 * @brief Returns an AvmResult indicating success and containing a value.
 *
 * @tparam T The type.
 * @param value The value.
 *
 * @return The AvmResult
 */
template <T>
AVMAPI AvmResult<T> AvmSuccess<T>(T value);

/**
 * @brief Returns an AvmResult indicating failure and containing a AvmError.
 *
 * @tparam T The type.
 * @param error The error.
 *
 * @return The AvmResult
 */
template <T>
AVMAPI AvmResult<T> AvmFailure<T>(AvmError* error);

/**
 * @brief Unwraps an AvmResult, returning either the contained value, or
 * calling AvmPanic.
 *
 * @tparam T The type.
 * @param self The AvmResult instance.
 *
 * @return The AvmResult value.
 */
template <T>
AVMAPI T AvmUnwrap<T>(AvmResult<T>* self);

/**
 * @brief Determines whether an AvmResult indicates failure.
 *
 * @tparam T The type.
 * @param self The AvmResult instance.
 *
 * @return true if the AvmResult represents failure, otherwise false.
 */
template <T>
AVMAPI bool AvmIsFailure<T>(AvmResult<T>* self);
#else
/**
 * @brief Returns an AvmResult indicating success and containing a value.
 *
 * @tparam T The type.
 * @param value The value.
 *
 * @return The AvmResult
 */
#    define AvmSuccess(T)   AVM_GENERIC(AvmSuccess, T)

/**
 * @brief Returns an AvmResult indicating failure and containing a AvmError.
 *
 * @param T The type.
 * @param error The error.
 *
 * @return The AvmResult
 */
#    define AvmFailure(T)   AVM_GENERIC(AvmFailure, T)

/**
 * @brief Unwraps an AvmResult, returning either the contained value, or calling
 * AvmPanic.
 *
 * @param T The type.
 * @param self The AvmResult instance.
 *
 * @return The AvmResult value.
 */
#    define AvmUnwrap(T)    AVM_GENERIC(AvmUnwrap, T)

/**
 * @brief Determines whether an AvmResult indicates failure.
 *
 * @param T The type.
 * @param self The AvmResult instance.
 *
 * @return true if the AvmResult represents failure, otherwise false.
 */
#    define AvmIsFailure(T) AVM_GENERIC(AvmIsFailure, T)
#endif  // DOXYGEN

/**
 * @brief Creates an AvmResult for a type.
 *
 * @param T The type.
 */
#define AVM_RESULT_TYPE(T)                                                     \
    AVM_INLINE_CLASS(AVM_GENERIC(AvmResult, T), object, {                      \
        AvmError* _error;                                                      \
        T _value;                                                              \
    });                                                                        \
                                                                               \
    static inline void AVM_GENERIC(AvmResultDestroy, T)(AvmResult(T) * self) { \
        if (self->_error != NULL) {                                            \
            AvmObjectDestroy(self->_error);                                    \
        }                                                                      \
    }                                                                          \
                                                                               \
    AVM_INLINE_TYPE(                                                           \
        AVM_GENERIC(AvmResult, T), object,                                     \
        {[FnEntryDtor] = (AvmFunction)AVM_GENERIC(AvmResultDestroy, T)});      \
                                                                               \
    static inline AvmResult(T) AvmSuccess(T)(T value) {                        \
        return (AvmResult(T)){                                                 \
            ._type = typeid(AVM_GENERIC(AvmResult, T)),                        \
            ._error = NULL,                                                    \
            ._value = value,                                                   \
        };                                                                     \
    }                                                                          \
                                                                               \
    static inline AvmResult(T) AvmFailure(T)(AvmError * error) {               \
        return (AvmResult(T)){                                                 \
            ._type = typeid(AVM_GENERIC(AvmResult, T)),                        \
            ._error = error,                                                   \
        };                                                                     \
    }                                                                          \
                                                                               \
    static inline T AvmUnwrap(T)(AvmResult(T) * self) {                        \
        if (self->_error == NULL) {                                            \
            return self->_value;                                               \
        }                                                                      \
                                                                               \
        AvmPanic("Tried to unwrap a result describing failure.");              \
    }                                                                          \
                                                                               \
    static inline bool AvmIsFailure(T)(AvmResult(T) * self) {                  \
        return self->_error != NULL;                                           \
    }

AVM_RESULT_TYPE(char)
AVM_RESULT_TYPE(byte)
AVM_RESULT_TYPE(short)
AVM_RESULT_TYPE(ushort)
AVM_RESULT_TYPE(int)
AVM_RESULT_TYPE(uint)
AVM_RESULT_TYPE(_long)
AVM_RESULT_TYPE(ulong)
AVM_RESULT_TYPE(bool)
AVM_RESULT_TYPE(str)
AVM_RESULT_TYPE(object)
AVM_RESULT_TYPE(size_t)
AVM_RESULT_TYPE(AvmString)
AVM_RESULT_TYPE(AvmFunction)

#ifndef AVM_MSVC
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wunused-variable"
#endif

AVM_INLINE_CLASS(AVM_GENERIC(AvmResult, void), object, { AvmError* _error; });

static inline void AVM_GENERIC(AvmResultDestroy, void)(AvmResult(void) * self) {
    if (self->_error != NULL) {
        AvmObjectDestroy(self->_error);
    }
}

AVM_INLINE_TYPE(AVM_GENERIC(AvmResult, void), object,
                {[FnEntryDtor] = (AvmFunction)AVM_GENERIC(AvmResultDestroy,
                                                          void)});

static inline AvmResult(void) AvmSuccess(void)(void) {
    return (AvmResult(void)){
        ._type = typeid(AVM_GENERIC(AvmResult, void)),
        ._error = NULL,
    };
}

static inline AvmResult(void) AvmFailure(void)(AvmError* error) {
    return (AvmResult(void)){
        ._type = typeid(AVM_GENERIC(AvmResult, void)),
        ._error = error,
    };
}

static inline void AvmUnwrap(void)(AvmResult(void) * self) {
    if (self->_error == NULL) {
        return;
    }

    AvmPanic("Tried to unwrap a result describing failure.");
}

static inline bool AvmIsFailure(void)(AvmResult(void) * self) {
    return self->_error != NULL;
}

#ifndef AVM_MSVC
#    pragma GCC diagnostic pop
#endif

#endif  // AVIUM_RESULT_H
