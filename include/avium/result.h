#ifndef AVIUM_RESULT_H
#define AVIUM_RESULT_H

#include "avium/types.h"
#include "avium/runtime.h"

#define AvmResult(T)    AVM_GENERIC(AvmResult, T)
#define AvmSuccess(T)   AVM_GENERIC(AvmSuccess, T)
#define AvmFailure(T)   AVM_GENERIC(AvmFailure, T)
#define AvmUnwrap(T)    AVM_GENERIC(AvmUnwrap, T)
#define AvmIsFailure(T) AVM_GENERIC(AvmIsFailure, T)

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

#endif  // AVIUM_RESULT_H
