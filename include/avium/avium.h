#ifndef AVIUM_H
#define AVIUM_H

#include "avium/types.h"
#include "avium/runtime.h"

AVMAPI void AvmRuntimeRunTests(str program);

#ifndef _AVM_BUILD
void __main();

int main(int argc, str argv[]) {
    AvmRuntimeInit(argc, argv);
    AvmRuntimeRunTests(argv[0]);
    __main();
    return 0;
}
#endif

#define main __main

#endif  // AVIUM_H
