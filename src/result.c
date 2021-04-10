#include "avium/result.h"
#include "avium/alloc.h"

#include "avium/runtime.h"
#include "avium/string.h"

#include <string.h>
#include <errno.h>

AVM_CLASS(AvmOSError, AvmError, { int _code; });
AVM_CLASS(AvmSimpleError, AvmError, { AvmErrorKind _kind; });

static AvmString AvmOSErrorToString(AvmOSError* self) {
    return AvmStringFrom(strerror(self->_code));
}

static AvmString AvmSimpleErrorToString(AvmSimpleError* self) {
    (void)self;
    return AvmStringFrom("");
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
