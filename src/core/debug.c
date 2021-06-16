#include "avium/core.h"

#include "avium/string.h"

#include <deps/gc.h>

#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef AvmDebugLog
#undef AvmDebugLog
#endif

void AvmDebugLog(AvmLocation location, str format, ...)
{
    va_list args;
    va_start(args, format);
    AvmVErrorf(format, args);
    va_end(args);
    AvmErrorf(" in %v\n", &location);
}

void* AvmDebugAlloc(AvmLocation location, uint size, bool containsPointers)
{
    AvmDebugLog(location, "Allocation of %u bytes", size);
    return AvmAlloc(size + AVM_DEBUG_SMASH_PROTECTOR_SIZE, containsPointers);
}

bool AvmDebuggerIsAttached(void)
{
    FILE* f = fopen("/proc/self/status", "r");

    char buffer[1024];

    for (uint i = 0; i < 8; i++)
    {
        fgets(buffer, 1024, f);
    }

    fclose(f);

    char entry[] = "TracerPid";

    assert(strstr(buffer, entry) != NULL);

    char* start = &buffer[sizeof(entry)];

    while (isspace(*start))
    {
        start++;
    }

    long long pid = strtoll(start, NULL, 10);
    return pid != 0;
}

void AvmDebuggerAttach(void)
{
    str name = AvmRuntimeGetProgramName();

    AvmString temp =
        AvmStringFormat("sudo gdb %s %u\n", strrchr(name, '/'), getpid());

    system(AvmStringToStr(&temp));
}

void AvmDebuggerBreak(void)
{
    raise(SIGTRAP);
}
