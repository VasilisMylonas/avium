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
        case ErrorKindArg:
            return AvmStringFrom("Received an invalid argument.");
        case ErrorKindRange:
            return AvmStringFrom("The provided index was out of range.");
        case ErrorKindMem:
            return AvmStringFrom(
                "There was not enough memory to handle the request.");
        case ErrorKindInvalidOp:
            return AvmStringFrom("The call was invalid for the current state.");
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

AvmError* AvmErrorFromOSCode(int code) {
    return heapalloc(AvmOSError, {
                                     ._type = AVM_GET_TYPE(AvmOSError),
                                     ._code = code,
                                 });
}

AvmError* AvmErrorGetLast(void) { return AvmErrorFromOSCode(errno); }

AvmError* AvmErrorOfKind(AvmErrorKind kind) {
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
        return AvmErrorFromOSCode(0);
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
