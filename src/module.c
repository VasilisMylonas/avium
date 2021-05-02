const int __AvmDummy = 0;

#include <avium/reflect.h>

void AvmRuntimeRunTests(str program) {
    AvmEnableExceptions();

    (void)program;
    // TODO

    AvmDisableExceptions();
}
