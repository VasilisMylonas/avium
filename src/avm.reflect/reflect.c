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

#ifdef AVM_LINUX
#include <stdio.h>
#include <string.h>

#define LINE_MAX_LENGTH 128

static const str Command = "cat /proc/self/maps | awk '!/\\[/ { print $6 }'";

void AvmReflectPrintLoadedLibs()
{
    FILE* f = popen(Command, "r");

    AvmString s = AvmStringNew(256);

    char previous[LINE_MAX_LENGTH];
    char buffer[LINE_MAX_LENGTH];

    for (size_t i = 0; true; i++)
    {
        if (fgets(buffer, LINE_MAX_LENGTH, f) == NULL)
        {
            break;
        }

        if (strlen(buffer) != 1 && strcmp(buffer, previous) != 0 && i > 0)
        {
            AvmStringPushStr(&s, buffer);
        }

        memcpy(previous, buffer, LINE_MAX_LENGTH);
    }

    AvmPrintf("%v", &s);
    AvmObjectDestroy(&s);
}
#endif
