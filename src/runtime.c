#include <signal.h>  // For signal and related constants
#include <stdlib.h>  // For exit
#include <string.h>  // For memcpy, strchr, strrchr

#include "avium/fmt.h"
#include "avium/runtime.h"
#include "avium/resources.h"

#include <stdlib.h>
#include <string.h>

#ifdef AVM_USE_GC
#    include <gc/gc.h>
#    define AVM_ALLOC   GC_malloc
#    define AVM_REALLOC GC_realloc
#    define AVM_DEALLOC GC_free
#else
#    include <stdlib.h>
#    define AVM_ALLOC   malloc
#    define AVM_REALLOC realloc
#    define AVM_DEALLOC free
#endif

void* AvmAlloc(size_t size) { return AVM_ALLOC(size); }
void* AvmRealloc(void* memory, size_t size) {
    return AVM_REALLOC(memory, size);
}
void AvmDealloc(void* memory) { AVM_DEALLOC(memory); }

object AvmObjectAlloc(size_t size, object data) {
    object memory = AvmAlloc(size);
    if (data != NULL) {
        memcpy(memory, data, size);
    }
    return memory;
}

static void ExceptionHandler(int exception) {
    switch (exception) {
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

void AvmInit(int argc, str argv[]) {
    (void)argc;
    (void)argv;
}

void AvmEnableExceptions(void) {
    signal(SIGSEGV, ExceptionHandler);
    signal(SIGILL, ExceptionHandler);
    signal(SIGFPE, ExceptionHandler);
}

void AvmDisableExceptions(void) {
    signal(SIGSEGV, SIG_DFL);
    signal(SIGILL, SIG_DFL);
    signal(SIGFPE, SIG_DFL);
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

#ifdef AVM_LINUX
#    include <execinfo.h>
#endif

never AvmPanicEx(str message, str function, str file, uint line) {
    AvmErrorf("Panic in file %s:%u in function %s()\n\n%s\n", file, line,
              function, message);

#ifdef AVM_LINUX
    object arr[BACKTRACE_MAX_SYMBOLS];

    int length = backtrace(arr, BACKTRACE_MAX_SYMBOLS);
    char** s = backtrace_symbols(arr, length);

    for (int i = length - 1; i >= 1; i--) {
        *(strrchr(s[i], ')')) = '\0';
        AvmErrorf("    at %s\n", strchr(s[i], '(') + 1);
    }
#else
    AvmErrorf("%s\n", NoBacktraceMsg);
#endif

    exit(1);
}
