#include "avium/reflect.h"

#include "avium/dlfcn.h"
#include "avium/string.h"
#include "avium/testing.h"

const AvmType* AvmReflectLoadType(str name)
{
    pre
    {
        assert(name != NULL);
    }

    AvmString temp = AvmStringFrom(name);
    uint index = AvmStringIndexOf(&temp, '@');

    char* buffer = AvmStringGetBuffer(&temp);
    buffer[index] = '\0';

    AvmString library = AvmStringFormat("lib%s.so", buffer);
    AvmString symbol = AvmStringFormat("_TI_%s", &buffer[index + 1]);

    void* handle = dlopen(AvmStringGetBuffer(&library), RTLD_LAZY);
    const AvmType* t = dlsym(handle, AvmStringGetBuffer(&symbol));

    AvmObjectDestroy(&library);
    AvmObjectDestroy(&symbol);
    AvmObjectDestroy(&temp);

    return t;
}
