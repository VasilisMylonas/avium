#ifndef AVIUM_THREAD_H
#define AVIUM_THREAD_H

#include "avium/core.h"

AVM_CLASS(AvmMutex, object, { void* _state; });

#define lock(mutex)                                                            \
    AvmMutexLock(mutex);                                                       \
    for (uint AVM_UNIQUE(__avmMC) = 0; AVM_UNIQUE(__avmMC) < 1;                \
         AVM_UNIQUE(__avmMC)++, AvmMutexUnlock(mutex))

AVMAPI AvmMutex AvmMutexNew();
AVMAPI void AvmMutexUnlock(const AvmMutex* self);
AVMAPI void AvmMutexLock(const AvmMutex* self);

AVM_CLASS(AvmBarrier, object, { void* _state; });

AVMAPI AvmBarrier AvmBarrierNew(uint count);
AVMAPI void AvmBarrierWait(const AvmBarrier* self);

AVM_CLASS(AvmThread, object, {
    AvmThrowContext* _context;
    void* _state;
    AvmMutex _lock;
    str _name;
    byte* _stackPtr;
    bool _isAlive;
    bool _isDetached;
    uint _stackSize;
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

AVMAPI byte* AvmThreadGetStackPtr(const AvmThread* self);
AVMAPI uint AvmThreadGetStackSize(const AvmThread* self);

#endif // AVIUM_THREAD_H
