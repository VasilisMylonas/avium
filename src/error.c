#include "avium/error.h"
#include "avium/alloc.h"

#include "avium/runtime.h"
#include "avium/string.h"

#include <string.h>
#include <errno.h>

AVM_CLASS(AvmCError, AvmError, { int _code; });

static AvmString AvmCErrorToString(AvmCError* self) {
    return AvmStringFrom(strerror(self->_code));
}

AVM_TYPE(AvmCError, {[FUNC_TO_STRING] = (AvmFunction)AvmCErrorToString});

AvmError* AvmErrorFromErrno(void) {
    return heapalloc(AvmCError, {
                                    ._type = AVM_GET_TYPE(AvmCError),
                                    ._code = errno,
                                });
}
