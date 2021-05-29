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

const AvmType* AvmObjectGetType(object self)
{
    pre
    {
        assert(self != NULL);
    }

    // The first member of an object should be a const AvmType*
    // Look in types.h for AVM_CLASS
    return *(const AvmType**)self;
}

//
// Virtual calls.
//

bool AvmObjectEquals(object self, object other)
{
    VIRTUAL_CALL(bool, FnEntryEquals, self, other);
}

void AvmObjectDestroy(object self)
{
    VIRTUAL_CALL(void, FnEntryDtor, self);
}

object AvmObjectClone(object self)
{
    VIRTUAL_CALL(object, FnEntryClone, self);
}

AvmString AvmObjectToString(object self)
{
    VIRTUAL_CALL(AvmString, FnEntryToString, self);
}

//
// Default implementations.
//

static void AvmObjectDestroyDefault(object self)
{
    pre
    {
        assert(self != NULL);
    }

    (void)self;
}

static bool AvmObjectEqualsDefault(object self, object other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    const size_t size = AvmTypeGetSize(AvmObjectGetType(self));
    return memcmp(self, other, size) == 0;
}

static object AvmObjectCloneDefault(object self)
{
    pre
    {
        assert(self != NULL);
    }

    uint size = AvmTypeGetSize(AvmObjectGetType(self));
    void* memory = AvmAlloc(size);
    AvmCopy(self, size, (byte*)memory);
    return memory;
}

static AvmString AvmObjectToStringDefault(object self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(
        "%s [%x]", AvmTypeGetName(AvmObjectGetType(self)), self);
}

//
// Type info for primitive types.
//

AVM_TYPE(object,
         object,
         {
             [FnEntryDtor] = (AvmFunction)AvmObjectDestroyDefault,
             [FnEntryEquals] = (AvmFunction)AvmObjectEqualsDefault,
             [FnEntryClone] = (AvmFunction)AvmObjectCloneDefault,
             [FnEntryToString] = (AvmFunction)AvmObjectToStringDefault,
         });

// TODO: Override functions.
AVM_TYPE(_long, object, {[FnEntryDtor] = NULL});
AVM_TYPE(ulong, object, {[FnEntryDtor] = NULL});
AVM_TYPE(int, object, {[FnEntryDtor] = NULL});
AVM_TYPE(uint, object, {[FnEntryDtor] = NULL});
AVM_TYPE(short, object, {[FnEntryDtor] = NULL});
AVM_TYPE(ushort, object, {[FnEntryDtor] = NULL});
AVM_TYPE(char, object, {[FnEntryDtor] = NULL});
AVM_TYPE(byte, object, {[FnEntryDtor] = NULL});
AVM_TYPE(str, object, {[FnEntryDtor] = NULL});
AVM_TYPE(float, object, {[FnEntryDtor] = NULL});
AVM_TYPE(double, object, {[FnEntryDtor] = NULL});

//
// AvmLocation type.
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

AvmLocation AvmLocationFrom(str file, uint line, uint column)
{
    pre
    {
        assert(file != NULL);
    }

    return (AvmLocation){
        ._type = typeid(AvmLocation),
        .File = file,
        .Line = line,
        .Column = column,
    };
}

//
// AvmVersion type.
//

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

//
// Builtin runtime functions.
//

static AvmString AvmRuntimeToString(const AvmRuntime* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat("%s v%v", self->_name, &self->_version);
}

AVM_TYPE(AvmRuntime,
         object,
         {
             [FnEntryToString] = (AvmFunction)AvmRuntimeToString,
         });

static thread_local AvmRuntime __AvmRuntimeState;

void AvmRuntimeInit(int argc, str argv[])
{
    __AvmRuntimeState._type = typeid(AvmRuntime);
    __AvmRuntimeState._argc = (uint)(argc - 1);
    __AvmRuntimeState._argv = argv + 1;
    __AvmRuntimeState._name = AVM_RUNTIME_NAME;
    __AvmRuntimeState._throwContext = NULL;
    __AvmRuntimeState._version =
        AvmVersionFrom(AVM_VERSION_MAJOR, AVM_VERSION_MINOR, AVM_VERSION_PATCH);
}

str AvmRuntimeGetProgramName(void)
{
    return __AvmRuntimeState._argv[0];
}

AvmVersion AvmRuntimeGetVersion(void)
{
    return __AvmRuntimeState._version;
}

uint AvmRuntimeGetArgCount(void)
{
    return __AvmRuntimeState._argc;
}

str* AvmRuntimeGetArgs(void)
{
    return __AvmRuntimeState._argv;
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

//
// Allocation functions.
//

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

//
// Fundamental IO.
//

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

void AvmVScanf(str format, va_list args)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
    }

    // TODO: Ideally we should have a custom implementation.
    vscanf(format, args);
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

// These just forward to the V- equivalents.

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
// Exception implementation.
//

AVM_TYPE(AvmThrowContext, object, {[FnEntryDtor] = NULL});

AvmThrowContext* __AvmRuntimeGetThrowContext(void)
{
    return __AvmRuntimeState._throwContext;
}

void __AvmRuntimePushThrowContext(AvmThrowContext* context)
{
    context->_type = typeid(AvmThrowContext);
    context->_thrownObject = NULL;
    context->_prev = __AvmRuntimeState._throwContext;
    __AvmRuntimeState._throwContext = context;
}

AvmThrowContext* __AvmRuntimePopThrowContext(void)
{
    AvmThrowContext* retval = __AvmRuntimeState._throwContext;
    __AvmRuntimeState._throwContext = retval->_prev;
    return retval;
}

never __AvmRuntimeThrow(object value, AvmLocation location)
{
    pre
    {
        assert(value != NULL);
    }

    __AvmRuntimeState._throwContext->_location = location;
    __AvmRuntimeState._throwContext->_thrownObject = value;
    longjmp(__AvmRuntimeState._throwContext->_jumpBuffer, 1);
}

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

// static void AvmHandleException(int exception)
// {
//     switch (exception)
//     {
//     case SIGSEGV:
//         AvmErrorf("%s\n", InvalidPtrDerefMsg);
//         break;
//     case SIGILL:
//         AvmErrorf("%s\n", IllegalInstructionMsg);
//         break;
//     case SIGFPE:
//         AvmErrorf("%s\n", ArithmeticExceptionMsg);
//         break;
//     default:
//         break;
//     }

//     exit(EXIT_FAILURE);
// }

// void AvmRuntimeEnableExceptions(void)
// {
//     signal(SIGSEGV, AvmHandleException);
//     signal(SIGILL, AvmHandleException);
//     signal(SIGFPE, AvmHandleException);
// }

// void AvmRuntimeDisableExceptions(void)
// {
//     signal(SIGSEGV, SIG_DFL);
//     signal(SIGILL, SIG_DFL);
//     signal(SIGFPE, SIG_DFL);
// }

// #ifdef AVM_USE_GC
// void AvmGCForceCollect(void)
// {
//     GC_gcollect();
// }

// void AvmGCDisable(void)
// {
//     GC_disable();
// }

// void AvmGCEnable(void)
// {
//     GC_enable();
// }

// ulong AvmGCGetTotalBytes(void)
// {
//     return GC_get_total_bytes();
// }

// ulong AvmGCGetFreeBytes(void)
// {
//     return GC_get_free_bytes();
// }

// ulong AvmGCGetUsedBytes(void)
// {
//     return GC_get_memory_use();
// }

// ulong AvmGCGetHeapSize(void)
// {
//     return GC_get_heap_size();
// }
// #endif
