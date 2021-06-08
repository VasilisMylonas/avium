#ifndef AVIUM_PRIVATE_THREAD_CONTEXT_H
#define AVIUM_PRIVATE_THREAD_CONTEXT_H

#include "avium/core.h"
#include "avium/thread.h"

AVM_CLASS(AvmThreadContext, object, {
    object _argument;
    AvmThreadEntryPoint _entryPoint;
    AvmThread* _thread;
});

AVMHIDDEN AvmThreadContext* AvmThreadContextNew(object argument,
                                                AvmThreadEntryPoint entryPoint,
                                                AvmThread* thread);

AVMHIDDEN AvmThread* AvmThreadContextGetThread(const AvmThreadContext* self);
AVMHIDDEN void AvmThreadContextEnter(const AvmThreadContext* self);
AVMHIDDEN AvmExitCode __AvmRuntimeThreadInit(AvmThreadContext* context);

AVMHIDDEN AvmThread* __AvmRuntimeCreateThreadObject(str name,
                                                    bool isDetached,
                                                    uint stackSize,
                                                    byte* stackPtr);

#endif // AVIUM_PRIVATE_THREAD_CONTEXT_H
