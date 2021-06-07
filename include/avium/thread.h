#ifndef AVIUM_THREAD_H
#define AVIUM_THREAD_H

#include "avium/core.h"

AVM_CLASS(AvmThread, object, {
    void* _state;
    void* _lock;
    AvmThrowContext* _context;
    str _name;
    volatile bool _isAlive;
    volatile bool _isDetached;
});

typedef void (*AvmThreadEntryPoint)(object);

AVMAPI AvmThread* AvmThreadNew(AvmThreadEntryPoint entry, object value);
AVMAPI AvmThread* AvmThreadNewEx(AvmThreadEntryPoint entry,
                                 object value,
                                 uint stackSize,
                                 byte* stackPtr,
                                 str name);

AVMAPI bool AvmThreadIsDetached(const AvmThread* self);
AVMAPI bool AvmThreadIsAlive(const AvmThread* self);
AVMAPI str AvmThreadGetName(const AvmThread* self);
AVMAPI uint AvmThreadGetCurrentID();
AVMAPI const AvmThread* AvmThreadGetCurrent();

AVMAPI AvmExitCode AvmThreadJoin(AvmThread* self);
AVMAPI void AvmThreadDetach(AvmThread* self);

AVMAPI void AvmThreadSleep(uint ms);
AVMAPI void AvmThreadYield();

AVMAPI never AvmThreadExit(AvmExitCode code);
AVMAPI never AvmThreadFastExit(AvmExitCode code);
AVMAPI void AvmThreadTerminate(AvmThread* self);

#endif // AVIUM_THREAD_H
