#ifndef AVIUM_PRIVATE_THREAD_CONTEXT_H
#define AVIUM_PRIVATE_THREAD_CONTEXT_H

#include "avium/core.h"
#include "avium/thread.h"

AVM_CLASS(AvmThreadContext, object, {
    object _argument;
    AvmThreadEntryPoint _entryPoint;
    AvmThread* _thread;
});

// Creates a new thread context.
extern AvmThreadContext* __AvmThreadContextNew(object argument,
                                               AvmThreadEntryPoint entryPoint,
                                               AvmThread* thread);

// Waits for the thread in the specified thread context to become available.
extern AvmThread* __AvmThreadContextGetThread(const AvmThreadContext* self,
                                              void* thread);

// Makes the current thread available in the specified thread context.
extern void __AvmThreadContextSetThread(AvmThreadContext* self);

// Begins executing in the specified thread context.
extern void __AvmThreadContextEnter(const AvmThreadContext* self);

// This is the first thing that runs in a newly created thread.
extern AvmExitCode __AvmRuntimeThreadInit(AvmThreadContext* context);

// Creates a thread object (just the object not the thread itself).
extern AvmThread* __AvmThreadNewObject(str name,
                                       bool isDetached,
                                       uint stackSize);

#endif // AVIUM_PRIVATE_THREAD_CONTEXT_H
