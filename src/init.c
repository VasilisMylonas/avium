#include "avium/core.h"
#include "avium/error.h"
#include <stdlib.h>

#undef main

// This should be defined by user code.
extern void AvmMain();

int main(int argc, str argv[])
{
    AvmRuntimeInit(argc, argv);

    AvmThrowContext context;
    __AvmRuntimePushThrowContext(&context);
    if (setjmp(context._jumpBuffer) == 0)
    {
        AvmMain();
    }
    else
    {
        AvmErrorf("Unhandled exception: TODO\n");
        exit(-1);
    }
    return 0;
}
