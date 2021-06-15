#ifndef AVIUM_PRIVATE_TASK_CONTEXT_H
#define AVIUM_PRIVATE_TASK_CONTEXT_H

#include "avium/task.h"

// Task context state.
AVM_CLASS(AvmTaskContext, object, {
    AvmTaskEntryPoint entry;
    object value;
});

// One time task subsystem init.
extern void __AvmRuntimeTaskInit();

// Per-thread task subsystem init.
extern void __AvmRuntimeThreadTaskInit();

// Creates a task context.
extern AvmTaskContext* __AvmTaskContextNew(AvmTaskEntryPoint entry,
                                           object value);

#endif // AVIUM_PRIVATE_TASK_CONTEXT_H
