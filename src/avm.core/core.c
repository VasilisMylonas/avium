#include "avium/core.h"

#include "avium/error.h"
#include "avium/private/resources.h"
#include "avium/string.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

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

static void ExceptionHandler(int exception)
{
    switch (exception)
    {
    case SIGSEGV:
        AvmPanic(InvalidPtrDerefMsg);
        break;
    case SIGILL:
        AvmPanic(IllegalInstructionMsg);
        break;
    case SIGFPE:
        AvmPanic(ArithmeticExceptionMsg);
        break;
    default:
        break;
    }
}

void AvmRuntimeEnableExceptions(void)
{
    signal(SIGSEGV, ExceptionHandler);
    signal(SIGILL, ExceptionHandler);
    signal(SIGFPE, ExceptionHandler);
}

void AvmRuntimeDisableExceptions(void)
{
    signal(SIGSEGV, SIG_DFL);
    signal(SIGILL, SIG_DFL);
    signal(SIGFPE, SIG_DFL);
}

static AvmEnv env;

weakptr(AvmEnv) AvmRuntimeInit(int argc, str argv[])
{
    (void)argc;

    // TODO
    env._isInitialized = true;
    env._programName = argv[0];
    env._args = &argv[1];
    env._version =
        AvmVersionFrom(AVM_VERSION_MAJOR, AVM_VERSION_MINOR, AVM_VERSION_PATCH);
    return &env;
}

str AvmRuntimeGetProgramName(void)
{
    return env._programName;
}

AvmVersion AvmRuntimeGetVersion(void)
{
    return env._version;
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
    fputs(AvmStringGetBuffer(&temp), stream);
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
