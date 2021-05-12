#include "avium/core.h"

#undef main

// This should be defined by user code.
extern void AvmMain();

int main(int argc, str argv[])
{
    AvmRuntimeInit(argc, argv);
    AvmMain();
    return 0;
}
