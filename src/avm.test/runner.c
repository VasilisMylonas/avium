#include "avium/runtime.h"

#include "avium/reflect.h"
#include "avium/string.h"

#include <dlfcn.h>
#include <stdio.h>

static const str CommandTemplate =
    "nm --defined-only -g %s | awk '/(__AvmTest)/ { print $3 }'";

void AvmRuntimeRunTests(str program) {
    AvmEnableExceptions();

    AvmArrayList(AvmString) symbols =
        __AvmGetSymbolList(program, CommandTemplate);

    void* handle = dlopen(program, RTLD_LAZY);

    for (size_t i = 0; i < symbols._length; i++) {
        void* func = dlsym(handle, AvmStringGetBuffer(&symbols._items[i]));

        if (func == NULL) {
            AvmPanic(dlerror());
        }

        (*(AvmFunction*)&func)();
    }

    AvmDisableExceptions();
}

int main(int argc, str argv[]) {
    AvmRuntimeInit(argc, argv);

    // AvmRuntimeRunTests(argv[1]);
    AvmRuntimeRunTests("/home/vasilis/Dev/avium/build/lib/libstring.so");
    return 0;
}
