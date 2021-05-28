#include "avium/core.h"

#include "avium/private/resources.h"
#include "avium/string.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef AVM_USE_GC
#include "gc.h"
#define AVM_ALLOC   GC_malloc
#define AVM_REALLOC GC_realloc
#define AVM_DEALLOC GC_free
#else
#define AVM_ALLOC   malloc
#define AVM_REALLOC realloc
#define AVM_DEALLOC free
#endif

#ifdef AVM_LINUX
#include <execinfo.h>
#endif

void* AvmAlloc(size_t size)
{
    return AVM_ALLOC(size);
}

void* AvmRealloc(void* memory, size_t size)
{
    return AVM_REALLOC(memory, size);
}

void AvmDealloc(void* memory)
{
    AVM_DEALLOC(memory);
}

static void AvmHandleException(int exception)
{
    switch (exception)
    {
    case SIGSEGV:
        AvmErrorf("%s\n", InvalidPtrDerefMsg);
        break;
    case SIGILL:
        AvmErrorf("%s\n", IllegalInstructionMsg);
        break;
    case SIGFPE:
        AvmErrorf("%s\n", ArithmeticExceptionMsg);
        break;
    default:
        break;
    }

    exit(EXIT_FAILURE);
}

static struct
{
    str* _args;
    AvmVersion _version;
} AvmState;

void AvmRuntimeInit(int argc, str argv[])
{
    (void)argc;

    AvmState._args = argv;
    AvmState._version =
        AvmVersionFrom(AVM_VERSION_MAJOR, AVM_VERSION_MINOR, AVM_VERSION_PATCH);
}

void AvmRuntimeEnableExceptions(void)
{
    signal(SIGSEGV, AvmHandleException);
    signal(SIGILL, AvmHandleException);
    signal(SIGFPE, AvmHandleException);
}

void AvmRuntimeDisableExceptions(void)
{
    signal(SIGSEGV, SIG_DFL);
    signal(SIGILL, SIG_DFL);
    signal(SIGFPE, SIG_DFL);
}

AvmString AvmRuntimeGetBacktrace(void)
{
#ifdef AVM_LINUX
    void* arr[BACKTRACE_MAX_SYMBOLS];

    int length = backtrace(arr, BACKTRACE_MAX_SYMBOLS);
    char** s = backtrace_symbols(arr, length);

    AvmString bt = AvmStringNew(length * 10);

    for (int i = length - 1; i >= 1; i--)
    {
        *(strchr(s[i], '+')) = '\0';
        *(strchr(s[i], '(')) = '@';

        AvmStringPushStr(&bt, "    in ");
        AvmStringPushStr(&bt, s[i]);
        if (i != 1)
        {
            AvmStringPushChar(&bt, '\n');
        }
    }

    free(s);
    return bt;
#else
    return AvmStringFrom(NoBacktraceMsg);
#endif
}

str AvmRuntimeGetProgramName(void)
{
    return AvmState._args[0];
}

AvmVersion AvmRuntimeGetVersion(void)
{
    return AvmState._version;
}

str* AvmRuntimeGetArgs(void)
{
    return AvmState._args + 1;
}

#ifdef AVM_USE_GC
void AvmGCForceCollect(void)
{
    GC_gcollect();
}

void AvmGCDisable(void)
{
    GC_disable();
}

void AvmGCEnable(void)
{
    GC_enable();
}

ulong AvmGCGetTotalBytes(void)
{
    return GC_get_total_bytes();
}

ulong AvmGCGetFreeBytes(void)
{
    return GC_get_free_bytes();
}

ulong AvmGCGetUsedBytes(void)
{
    return GC_get_memory_use();
}

ulong AvmGCGetHeapSize(void)
{
    return GC_get_heap_size();
}
#endif

void AvmCopy(object o, size_t size, byte* destination)
{
    pre
    {
        assert(o != NULL);
        assert(destination != NULL);
    }

    size_t objectSize = AvmTypeGetSize(AvmObjectGetType(o));

    if (objectSize > size)
    {
        // TODO: error
    }

    memcpy(destination, o, objectSize);
}

void AvmVScanf(str format, va_list args)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
    }

    // TODO
    vscanf(format, args);
}

static void AvmFputs(str format, va_list args, FILE* stream)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
        assert(stream != NULL);
    }

    AvmString temp = AvmStringFormatV(format, args);
    fwrite(AvmStringGetBuffer(&temp), sizeof(char), temp._length, stream);
    AvmObjectDestroy(&temp);
}

void AvmVPrintf(str format, va_list args)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
    }

    AvmFputs(format, args, stdout);
}

void AvmVErrorf(str format, va_list args)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
    }

    AvmFputs(format, args, stderr);
}

void AvmScanf(str format, ...)
{
    pre
    {
        assert(format != NULL);
    }

    va_list args;
    va_start(args, format);
    AvmVScanf(format, args);
    va_end(args);
}

void AvmPrintf(str format, ...)
{
    pre
    {
        assert(format != NULL);
    }

    va_list args;
    va_start(args, format);
    AvmVPrintf(format, args);
    va_end(args);
}

void AvmErrorf(str format, ...)
{
    pre
    {
        assert(format != NULL);
    }

    va_list args;
    va_start(args, format);
    AvmVErrorf(format, args);
    va_end(args);
}

static AvmString AvmVersionToString(AvmVersion* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat("%i.%i.%i", self->Major, self->Minor, self->Patch);
}

AVM_TYPE(AvmVersion,
         object,
         {[FnEntryToString] = (AvmFunction)AvmVersionToString});

AvmVersion AvmVersionFrom(ushort major, ushort minor, ushort patch)
{
    return (AvmVersion){
        ._type = typeid(AvmVersion),
        .Major = major,
        .Minor = minor,
        .Patch = patch,
    };
}

#define VA_LIST_TO_ARRAY_IMPL(T1, T2)                                          \
    for (size_t i = 0; i < length; i++)                                        \
    {                                                                          \
        ((T1*)stack)[i] = (T1)va_arg(args, T2);                                \
    }                                                                          \
                                                                               \
    return stack;

void* __AvmVaListToArray(void* stack, va_list args, uint size, uint length)
{
    pre
    {
        assert(stack != NULL);
        assert(args != NULL);
        assert(size != 0);
        assert(length != 0);
    }

    switch (size)
    {
    case 1:
        VA_LIST_TO_ARRAY_IMPL(byte, uint);
    case 2:
        VA_LIST_TO_ARRAY_IMPL(ushort, uint);
    case 4:
        VA_LIST_TO_ARRAY_IMPL(uint, uint);
    case 8:
        VA_LIST_TO_ARRAY_IMPL(ulong, ulong);
    default:
        return NULL; // TODO: ERROR
    }
}

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
