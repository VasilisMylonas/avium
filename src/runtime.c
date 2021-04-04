#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "avium/runtime.h"
#include "avium/resources.h"

static void ExceptionHandler(int exception) {
    switch (exception) {
        case SIGSEGV:
            AvmPanic("Invalid pointer dereference.");
            break;
        case SIGILL:
            AvmPanic("Illegal instruction.");
            break;
        case SIGFPE:
            AvmPanic("Arithmetic exception.");
            break;
        case SIGINT:
            AvmPanic("Received interrupt.");
            break;
        default:
            break;
    }
}

void AvmRuntimeEnableExceptions(void) {
    signal(SIGSEGV, ExceptionHandler);
    signal(SIGILL, ExceptionHandler);
    signal(SIGFPE, ExceptionHandler);
    signal(SIGINT, ExceptionHandler);
}

void AvmRuntimeDisableExceptions(void) {
    signal(SIGSEGV, SIG_DFL);
    signal(SIGILL, SIG_DFL);
    signal(SIGFPE, SIG_DFL);
    signal(SIGINT, SIG_DFL);
}

void AvmMemCopy(byte* source, size_t length, byte* destination, size_t size) {
    if (source == NULL) {
        AvmPanic(SourceNullMsg);
    }

    if (destination == NULL) {
        AvmPanic(DestinationNullMsg);
    }

    size_t trueLength = length > size ? size : length;
    memcpy(destination, source, trueLength);
}

#ifdef AVM_LINUX
#    include <execinfo.h>
#endif

never AvmPanicEx(str message, str function, str file, uint line) {
    fprintf(stderr, "Panic in file %s:%u in function %s()\n\n%s\n", file, line,
            function, message);

#ifdef AVM_LINUX
    object arr[128];

    int length = backtrace(arr, 128);
    char** s = backtrace_symbols(arr, length);

    for (int i = length - 1; i >= 1; i--) {
        *(strrchr(s[i], ')')) = '\0';
        fprintf(stderr, "    at %s\n", strchr(s[i], '(') + 1);
    }
#else
    fprintf(stderr, "No backtrace is available.\n");
#endif

    exit(1);
}

str AvmTypeGetName(const AvmType* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_name;
}

size_t AvmTypeGetSize(const AvmType* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_size;
}
