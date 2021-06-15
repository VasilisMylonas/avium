#ifndef AVIUM_TASK_H
#define AVIUM_TASK_H

#include "avium/core.h"

#define AVM_TASK_STACK_SIZE (4096 * 16)

AVM_CLASS(AvmTask, object, {
    struct
    {
        const AvmTask* previous;
        void* state;
        void* stack;
        str name;
        object retval;
        uint id;
    } _private;
});

/// If this is returned, it means the task completed.
#define AVM_TASK_EXITED ((void*)1)

typedef void (*AvmTaskEntryPoint)(object);

AVMAPI AvmTask* AvmTaskNew(AvmTaskEntryPoint entry, object value);
AVMAPI AvmTask* AvmTaskNewEx(AvmTaskEntryPoint entry,
                             object value,
                             uint stackSize,
                             str name);

AVMAPI const AvmTask* AvmTaskGetMain(void);
AVMAPI const AvmTask* AvmTaskGetCurrent(void);
AVMAPI str AvmTaskGetName(const AvmTask* self);
AVMAPI uint AvmTaskGetID(const AvmTask* self);
AVMAPI object AvmTaskSwitchTo(const AvmTask* self);
AVMAPI void AvmTaskSwitchBack(void);
AVMAPI void AvmTaskReturn(object value);

AVMAPI object AvmTaskRun(AvmTaskEntryPoint entry, object value);
AVMAPI never AvmTaskExit(void);

#ifdef AVM_USE_COROUTINE_KEYWORDS
#define co_switch AvmTaskSwitchTo
#define co_yield AvmTaskReturn
#define co_back AvmTaskSwitchBack
#define co_run  AvmTaskRun
#define co_exit return
#endif

#endif // AVIUM_TASK_H
