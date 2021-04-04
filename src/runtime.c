#include "avium/runtime.h"
#include <signal.h>
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

never AvmRuntimeVirtualFunctionTrap(void) {
    AvmPanic(VirtualFuncTrapTriggered);
}
