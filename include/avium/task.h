#ifndef AVIUM_TASK_H
#define AVIUM_TASK_H

#include "avium/core.h"

typedef void (*AvmTaskEntryPoint)(object);

#define AVM_TASK_STACK_SIZE 1048576 // 1 megabyte

AVM_CLASS(AvmTask, object, {
    struct
    {
        void* state;
    } _private;
});

AVMAPI AvmTask AvmTaskNew(AvmTaskEntryPoint entry, object value);

#endif // AVIUM_TASK_H
