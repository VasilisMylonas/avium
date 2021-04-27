#include "avium/error.h"

#include "avium/runtime.h"
#include "avium/string.h"
#include "avium/private/resources.h"

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
        case ErrorKindRead:
            return AvmStringFrom("Could not perform the read operation.");
        case ErrorKindWrite:
            return AvmStringFrom("Could not perform the write operation.");
        default:
            return AvmStringNew(0);
    }
}

AVM_TYPE(AvmOSError, {[FnEntryToString] = (AvmFunction)AvmOSErrorToString});
AVM_TYPE(AvmSimpleError,
         {[FnEntryToString] = (AvmFunction)AvmSimpleErrorToString});

AvmError* AvmErrorFromOSCode(int code) {
    return heapalloc(AvmOSError, {
                                     ._type = typeid(AvmOSError),
                                     ._code = code,
                                 });
}

AvmError* AvmErrorGetLast(void) { return AvmErrorFromOSCode(errno); }

AvmError* AvmErrorOfKind(AvmErrorKind kind) {
    return heapalloc(AvmSimpleError, {
                                         ._type = typeid(AvmSimpleError),
                                         ._kind = kind,
                                     });
}

AvmError* AvmErrorGetSource(AvmError* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntryGetSource];

    if (func != NULL) {
        return ((AvmError * (*)(AvmError*)) func)(self);
    }

    return AvmErrorFromOSCode(0);
}

AvmString AvmErrorGetBacktrace(AvmError* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntryGetBacktrace];

    if (func != NULL) {
        return ((AvmString(*)(AvmError*))func)(self);
    }

    return AvmStringNew(0);
}
