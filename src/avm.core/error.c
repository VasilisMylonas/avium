#include "avium/error.h"

#include "avium/core.h"
#include "avium/string.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

//
// AvmNativeError
//

AVM_CLASS(AvmNativeError, object, { int _code; });

static AvmString AvmNativeErrorToString(const AvmNativeError* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFrom(strerror(self->_code));
}

AVM_TYPE(AvmNativeError,
         object,
         {
             [FnEntryToString] = (AvmFunction)AvmNativeErrorToString,
         });

AvmError* AvmErrorFromOSCode(int code)
{
    AvmNativeError* e = AvmAlloc(sizeof(AvmNativeError));
    e->_type = typeid(AvmNativeError);
    e->_code = code;
    return e;
}

//
// AvmDetailedError
//

AVM_CLASS(AvmDetailedError, object, { str _message; });

static AvmString AvmDetailedErrorToString(const AvmDetailedError* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFrom(self->_message);
}

AVM_TYPE(AvmDetailedError,
         object,
         {
             [FnEntryToString] = (AvmFunction)AvmDetailedErrorToString,
         });

AvmError* AvmErrorNew(str message)
{
    pre
    {
        assert(message != NULL);
    }

    AvmDetailedError* e = AvmAlloc(sizeof(AvmDetailedError));
    e->_type = typeid(AvmDetailedError);
    e->_message = message;
    return e;
}

never AvmPanicEx(str message, str function, str file, uint line)
{
    AvmErrorf("Panic in file %s:%u in function %s()\n\n%s\n",
              file,
              line,
              function,
              message);

    AvmString s = AvmRuntimeGetBacktrace();
    AvmErrorf("%v\n", &s);
    exit(1);
}

static thread_local AvmThrowCallback AvmThrowHandler;

void AvmCatch(AvmThrowCallback handler)
{
    AvmThrowHandler = handler;
}

void AvmThrow(object value)
{
    AvmString s = AvmRuntimeGetBacktrace();

    if (AvmThrowHandler != NULL)
    {
        AvmThrowHandler(value, s);
    }

    AvmErrorf("Uncaught object of type %T.\n\n%v\n%v\n", value, value, &s);
    exit(EXIT_FAILURE);
}
