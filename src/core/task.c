#include "avium/task.h"

#include "avium/typeinfo.h"

#include <stdlib.h>

#include <deps/gc.h>
#include <deps/task.h>

AVM_CLASS_TYPE(AvmTask, object, AVM_VTABLE_DEFAULT);

AvmTask AvmTaskNew(AvmTaskEntryPoint entry, object value)
{
    // TODO: This should be available as a public API.
    void* stack = GC_malloc_uncollectable(AVM_TASK_STACK_SIZE);

    // This should not contain pointers to GC memory right?
    coro_context* context = AvmAlloc(sizeof(context), false);
    // TODO: Maybe decrease size and add overflow protection.
    coro_create(context, entry, value, stack, AVM_TASK_STACK_SIZE);

    return (AvmTask){
        .__type = typeid(AvmTask),
        ._private =
            {
                .state = context,
            },
    };
}
