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

box(void) AvmAlloc(size_t size)
{
    return AVM_ALLOC(size);
}

box(void) AvmRealloc(box(void) memory, size_t size)
{
    return AVM_REALLOC(memory, size);
}

void AvmDealloc(box(void) memory)
{
    AVM_DEALLOC(memory);
}

static void AvmHandleException(int exception)
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
