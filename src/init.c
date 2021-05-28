#include "avium/core.h"
#include "avium/error.h"

#undef main

// This should be defined by user code.
extern void AvmMain();

int main(int argc, str argv[])
{
    AvmRuntimeInit(argc, argv);

    try
    {
        AvmMain();
    }
    catch (object, e)
    {
        AvmErrorf("Unhandled exception: TODO\n");
        AvmErrorf("%v\n", e);
        return 1;
    }

    return 0;
}
