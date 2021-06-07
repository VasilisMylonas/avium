#ifndef AVIUM_PRIVATE_THREAD_CONTEXT_H
#define AVIUM_PRIVATE_THREAD_CONTEXT_H

#include "avium/core.h"
#include "avium/testing.h"
#include "avium/thread.h"
#include "avium/typeinfo.h"

AVM_CLASS(AvmThreadContext, object, {
    object _argument;
    AvmThreadEntryPoint _entryPoint;
    AvmThread* _thread;
});
AvmThreadContext* AvmThreadContextNew(object argument,
                                      AvmThreadEntryPoint entryPoint);
AvmThread* AvmThreadContextGetThread(const AvmThreadContext* self);
void AvmThreadContextEnter(const AvmThreadContext* self);
AvmExitCode __AvmRuntimeThreadInit(AvmThreadContext* context);

#endif // AVIUM_PRIVATE_THREAD_CONTEXT_H
