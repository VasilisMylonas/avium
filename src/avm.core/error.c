#include "avium/error.h"

#include "avium/string.h"
#include "avium/typeinfo.h"
#include "avium/testing.h"

#include "avium/private/resources.h"
#include "avium/private/errors.h"

#include <string.h>
#include <errno.h>
#include <stdlib.h>

#ifdef AVM_LINUX
#include <execinfo.h>
#endif

never AvmPanicEx(str message, str function, str file, uint line)
{
    AvmErrorf("Panic in file %s:%u in function %s()\n\n%s\n", file, line,
              function, message);

#ifdef AVM_LINUX
    void *arr[BACKTRACE_MAX_SYMBOLS];

    int length = backtrace(arr, BACKTRACE_MAX_SYMBOLS);
    char **s = backtrace_symbols(arr, length);

    for (int i = length - 1; i >= 1; i--)
    {
        *(strrchr(s[i], ')')) = '\0';
        AvmErrorf("    at %s\n", strchr(s[i], '(') + 1);
    }
#else
    AvmErrorf("%s\n", NoBacktraceMsg);
#endif

    exit(1);
}

//
// AvmOSError specific.
//

AVM_CLASS(AvmOSError, AvmError, { int _code; });

static AvmString AvmOSErrorToString(AvmOSError *self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFrom(strerror(self->_code));
}

AVM_TYPE(AvmOSError, object,
         {[FnEntryToString] = (AvmFunction)AvmOSErrorToString});

AvmError *AvmErrorFromOSCode(int code)
{
    AvmOSError *error = AvmAlloc(sizeof(AvmOSError));
    error->_type = typeid(AvmOSError);
    error->_code = code;
    return (AvmError *)error;
}

AvmError *AvmErrorGetLast(void) { return AvmErrorFromOSCode(errno); }

//
// AvmSimpleError specific.
//

AVM_CLASS(AvmSimpleError, AvmError, { AvmErrorKind _kind; });

static AvmString AvmSimpleErrorToString(AvmSimpleError *self)
{
    pre
    {
        assert(self != NULL);
    }

    switch (self->_kind)
    {
    case ErrorKindArg:
        return AvmStringFrom(ArgError);
    case ErrorKindRange:
        return AvmStringFrom(RangeError);
    case ErrorKindMem:
        return AvmStringFrom(MemError);
    case ErrorKindInvalidOp:
        return AvmStringFrom(InvalidOpError);
    case ErrorKindIO:
        return AvmStringFrom(IOError);
    case ErrorKindSys:
        return AvmStringFrom(SysError);
    case ErrorKindNotFound:
        return AvmStringFrom(NotFoundError);
    case ErrorKindRead:
        return AvmStringFrom(ReadError);
    case ErrorKindWrite:
        return AvmStringFrom(WriteError);
    default:
        return AvmStringNew(0);
    }
}

AVM_TYPE(AvmSimpleError, object,
         {[FnEntryToString] = (AvmFunction)AvmSimpleErrorToString});

AvmError *AvmErrorOfKind(AvmErrorKind kind)
{
    AvmSimpleError *error = AvmAlloc(sizeof(AvmSimpleError));
    error->_type = typeid(AvmSimpleError);
    error->_kind = kind;
    return (AvmError *)error;
}

//
// Virtual functions.
//

static const AvmOSError NoError = {
    ._type = typeid(AvmOSError),
    ._code = 0,
};

weakptr(AvmError) AvmErrorGetSource(AvmError *self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntryGetSource];

    // No need to panic if there is not GetSource function.
    if (func == NULL)
    {
        // The return type is weakptr(AvmError) so the user should know not to
        // modify this value.
        return (AvmError *)&NoError;
    }

    return ((AvmError * (*)(AvmError *)) func)(self);
}

AvmString AvmErrorGetBacktrace(AvmError *self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntryGetBacktrace];

    // A backtrace may not be supported.
    if (func == NULL)
    {
        return AvmStringNew(0);
    }

    return ((AvmString(*)(AvmError *))func)(self);
}
