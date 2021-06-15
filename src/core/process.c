#include "avium/process.h"

#include "avium/typeinfo.h"

#include <stdlib.h>

AVM_CLASS_TYPE(AvmProcess, object, AVM_VTABLE_DEFAULT);

void AvmProcessExit(AvmExitCode code)
{
    exit(code);
}

void AvmProcessFastExit(AvmExitCode code)
{
    (void)code;
    // TODO: macos does not have quick_exit?
    abort();
    // quick_exit(code);
}
