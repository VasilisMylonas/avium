#ifndef AVIUM_DEBUG_H
#define AVIUM_DEBUG_H

#include "avium/core.h"

AVMAPI void AvmDebugLog(AvmLocation location, str format, ...);
AVMAPI void* AvmDebugAlloc(AvmLocation, uint size, bool containsPointers);

AVMAPI void AvmDebuggerAttach(void);
AVMAPI bool AvmDebuggerIsAttached(void);
AVMAPI void AvmDebuggerBreak(void);

#ifdef AVM_DEBUG
#define AvmAlloc(size, containsPointers)                                       \
    AvmDebugAlloc(                                                             \
        AvmLocationFrom(__FILE__, __LINE__, 0), size, containsPointers)
#define AvmDebugLog(...)                                                       \
    AvmDebugLog(AvmLocationFrom(__FILE__, __LINE__, 0), __VA_ARGS__)
#else
#define AvmDebugLog(...) ((void)0)
#endif

#endif // AVIUM_DEBUG_H
