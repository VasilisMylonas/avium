#include "avium/result.h"

#include "avium/alloc.h"
#include "avium/runtime.h"
#include "avium/string.h"
#include "avium/resources.h"

#include <string.h>
#include <errno.h>

AVM_CLASS(AvmOSError, AvmError, { int _code; });
AVM_CLASS(AvmSimpleError, AvmError, { AvmErrorKind _kind; });

static AvmString AvmOSErrorToString(AvmOSError* self) {
    return AvmStringFrom(strerror(self->_code));
}

static AvmString AvmSimpleErrorToString(AvmSimpleError* self) {
    switch (self->_kind) {
        case ErrorKindNone:
            return AvmStringFrom("No error occurred.");
        case ErrorKindArg:
            return AvmStringFrom(
                "An invalid argument was received by a function.");
        case ErrorKindRange:
            return AvmStringFrom("A provided index was out of range.");
        case ErrorKindMem:
            return AvmStringFrom(
                "There was not enough memory to handle an operation.");
        case ErrorKindInvalidOp:
            return AvmStringFrom(
                "A function call was invalid for the current state.");
        case ErrorKindIO:
            return AvmStringFrom("An IO error occurred.");
        case ErrorKindSys:
            return AvmStringFrom("An unknown system error occurred.");
        case ErrorKindNotFound:
            return AvmStringFrom("A required resource was unavailable.");
        default:
            return AvmStringNew(0);
    }
}

AVM_TYPE(AvmOSError, {[FUNC_TO_STRING] = (AvmFunction)AvmOSErrorToString});
AVM_TYPE(AvmSimpleError,
         {[FUNC_TO_STRING] = (AvmFunction)AvmSimpleErrorToString});

AvmError* AvmErrorNewOSError(int code) {
    return heapalloc(AvmOSError, {
                                     ._type = AVM_GET_TYPE(AvmOSError),
                                     ._code = code,
                                 });
}

AvmError* AvmErrorGetLast(void) { return AvmErrorNewOSError(errno); }

AvmError* AvmErrorNewSimpleError(AvmErrorKind kind) {
    return heapalloc(AvmSimpleError, {
                                         ._type = AVM_GET_TYPE(AvmSimpleError),
                                         ._kind = kind,
                                     });
}

AvmError* AvmErrorGetSource(AvmError* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self->_source == NULL) {
        return AvmErrorNewOSError(0);
    }

    return self->_source;
}

AvmString AvmErrorGetBacktrace(AvmError* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FUNC_GET_BACKTRACE];

    if (func != NULL) {
        return ((AvmString(*)(AvmError*))func)(self);
    }

    return AvmStringNew(0);
}
