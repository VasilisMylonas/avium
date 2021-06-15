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
    } _private;
});

typedef void (*AvmTaskEntryPoint)(object);

AVMAPI const AvmTask* AvmTaskNew(AvmTaskEntryPoint entry, object value);
AVMAPI const AvmTask* AvmTaskGetMain();
AVMAPI const AvmTask* AvmTaskGetCurrent();
AVMAPI void AvmTaskSwitchTo(const AvmTask* self);
AVMAPI void AvmTaskSwitchBack();

#endif // AVIUM_TASK_H
