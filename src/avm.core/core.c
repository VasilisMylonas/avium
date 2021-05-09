#include <signal.h> // For signal and related constants
#include <stdlib.h> // For exit
#include <string.h> // For memcpy, strchr, strrchr
#include <stdio.h>  // For vfprintf, vscanf, stderr, stdout

#include "avium/core.h"
#include "avium/string.h"
#include "avium/error.h"
#include "avium/typeinfo.h"
#include "avium/private/resources.h"

#ifdef AVM_USE_GC
#include "gc.h"
#define AVM_ALLOC GC_malloc
#define AVM_REALLOC GC_realloc
#define AVM_DEALLOC GC_free
#else
#define AVM_ALLOC malloc
#define AVM_REALLOC realloc
#define AVM_DEALLOC free
#endif

box(void) AvmAlloc(size_t size) { return AVM_ALLOC(size); }
box(void) AvmRealloc(box(void) memory, size_t size)
{
    return AVM_REALLOC(memory, size);
}
void AvmDealloc(box(void) memory) { AVM_DEALLOC(memory); }

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

void AvmEnableExceptions(void)
{
    signal(SIGSEGV, ExceptionHandler);
    signal(SIGILL, ExceptionHandler);
    signal(SIGFPE, ExceptionHandler);
}

void AvmDisableExceptions(void)
{
    signal(SIGSEGV, SIG_DFL);
    signal(SIGILL, SIG_DFL);
    signal(SIGFPE, SIG_DFL);
}

void AvmMemCopy(byte *source, size_t length, byte *destination, size_t size)
{
    if (source == NULL)
    {
        AvmPanic(SourceNullMsg);
    }

    if (destination == NULL)
    {
        AvmPanic(DestinationNullMsg);
    }

    size_t trueLength = length > size ? size : length;
    memcpy(destination, source, trueLength);
}

static str ProgramName = NULL;

str AvmRuntimeGetProgramName(void)
{
    if (ProgramName == NULL)
    {
        AvmPanic("Runtime not initialized! Call AvmRuntimeInit first!");
    }

    return ProgramName;
}

void AvmRuntimeInit(int argc, str argv[])
{
    (void)argc;
    ProgramName = argv[0];
}

void AvmVScanf(str format, va_list args)
{
    if (format == NULL)
    {
        AvmPanic(FormatNullMsg);
    }

    // TODO
    vscanf(format, args);
}

void AvmVPrintf(str format, va_list args)
{
    if (format == NULL)
    {
        AvmPanic(FormatNullMsg);
    }

    AvmString temp = AvmStringFormatV(format, args);
    fputs(AvmStringGetBuffer(&temp), stdout);
    AvmObjectDestroy(&temp);
}

void AvmVErrorf(str format, va_list args)
{
    if (format == NULL)
    {
        AvmPanic(FormatNullMsg);
    }

    AvmString temp = AvmStringFormatV(format, args);
    fputs(AvmStringGetBuffer(&temp), stderr);
    AvmObjectDestroy(&temp);
}

void AvmScanf(str format, ...)
{
    if (format == NULL)
    {
        AvmPanic(FormatNullMsg);
    }
    AVM_FORWARD(format, AvmVScanf);
}

void AvmPrintf(str format, ...)
{
    if (format == NULL)
    {
        AvmPanic(FormatNullMsg);
    }
    AVM_FORWARD(format, AvmVPrintf);
}

void AvmErrorf(str format, ...)
{
    if (format == NULL)
    {
        AvmPanic(FormatNullMsg);
    }
    AVM_FORWARD(format, AvmVErrorf);
}
