#ifndef AVIUM_RESULT_H
#define AVIUM_RESULT_H

#include "avium/types.h"
#include "avium/runtime.h"

typedef struct AvmError AvmError;
struct AvmError {
    AvmType* _type;
    AvmError* _source;
};

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
} AvmErrorKind;

AVMAPI AvmError* AvmErrorGetLast(void);
AVMAPI AvmError* AvmErrorNewOSError(int code);
AVMAPI AvmError* AvmErrorNewSimpleError(AvmErrorKind kind);
AVMAPI AvmError* AvmErrorGetSource(AvmError* self);
AVMAPI AvmString AvmErrorGetBacktrace(AvmError* self);

#define AvmResult(T)    AVM_GENERIC(AvmResult, T)
#define AvmSuccess(T)   AVM_GENERIC(AvmSuccess, T)
#define AvmFailure(T)   AVM_GENERIC(AvmFailure, T)
#define AvmUnwrap(T)    AVM_GENERIC(AvmUnwrap, T)
#define AvmIsFailure(T) AVM_GENERIC(AvmIsFailure, T)

#define AVM_RESULT_TYPE(T)                                                     \
    AVM_CLASS(AVM_GENERIC(AvmResult, T), object, {                             \
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
    AVM_TYPE(AVM_GENERIC(AvmResult, T),                                        \
             {[FUNC_DTOR] = (AvmFunction)AVM_GENERIC(AvmResultDestroy, T)});   \
                                                                               \
    static inline AvmResult(T) AvmSuccess(T)(T value) {                        \
        return (AvmResult(T)){                                                 \
            ._type = AVM_GET_TYPE(AVM_GENERIC(AvmResult, T)),                  \
            ._error = NULL,                                                    \
            ._value = value,                                                   \
        };                                                                     \
    }                                                                          \
                                                                               \
    static inline AvmResult(T) AvmFailure(T)(AvmError * error) {               \
        return (AvmResult(T)){                                                 \
            ._type = AVM_GET_TYPE(AVM_GENERIC(AvmResult, T)),                  \
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

#endif  // AVIUM_RESULT_H
