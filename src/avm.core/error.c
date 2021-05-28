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

//
// AvmLocation
//

static AvmString AvmLocationToString(AvmLocation* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat("%s:%u", self->File, self->Line);
}

AVM_TYPE(AvmLocation,
         object,
         {[FnEntryToString] = (AvmFunction)AvmLocationToString});

static thread_local AvmThrowContext* AvmGlobalThrowContext;

AVM_TYPE(AvmThrowContext, object, {[FnEntryDtor] = NULL});

AvmThrowContext* __AvmRuntimeGetThrowContext(void)
{
    return AvmGlobalThrowContext;
}

void __AvmRuntimePushThrowContext(AvmThrowContext* context)
{
    context->_type = typeid(AvmThrowContext);
    context->_thrownObject = NULL;
    context->_prev = AvmGlobalThrowContext;
    AvmGlobalThrowContext = context;
}

AvmThrowContext* __AvmRuntimePopThrowContext(void)
{
    AvmThrowContext* retval = AvmGlobalThrowContext;
    AvmGlobalThrowContext = retval->_prev;
    return retval;
}

never __AvmRuntimeThrow(object value)
{
    pre
    {
        assert(value != NULL);
    }

    AvmGlobalThrowContext->_thrownObject = value;
    longjmp(AvmGlobalThrowContext->_jumpBuffer, 1);
}
