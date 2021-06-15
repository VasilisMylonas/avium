#include "avium/task.h"

#include "avium/thread.h"
#include "avium/typeinfo.h"

#include "avium/private/task-context.h"
#include <stdlib.h>

#include <deps/gc.h>
#include <deps/task.h>

static void AvmTaskFinalize(AvmTask* self)
{
    (void)coro_destroy(self->_private.state);
}

AVM_CLASS_TYPE(AvmTask,
               object,
               {
                   [AvmEntryFinalize] = (AvmCallback)AvmTaskFinalize,
               });

static thread_local const AvmTask* AvmCurrentTask = NULL;
static thread_local AvmTask AvmMainTask;
static AvmMutex AvmTaskMutex;

AVM_CLASS_TYPE(AvmTaskContext, object, AVM_VTABLE_DEFAULT);

AvmTaskContext* __AvmTaskContextNew(AvmTaskEntryPoint entry, object value)
{
    AvmTaskContext* context = AvmObjectNew(typeid(AvmTaskContext));
    context->entry = entry;
    context->value = value;
    return context;
}

// This is used to automatically call AvmTaskSwitchBack on return.
static void AvmTaskForwarder(AvmTaskContext* context)
{
    context->entry(context->value);
    AvmTaskSwitchBack();
}

void __AvmRuntimeTaskInit()
{
    AvmTaskMutex = AvmMutexNew(false);
}

void __AvmRuntimeThreadTaskInit()
{
    // same as AvmTaskNew.
    coro_context* context = AvmAlloc(sizeof(coro_context), false);

    // According to documentation this is a special case.
    // Also TODO: Not reentrant.
    coro_create(context, NULL, NULL, NULL, 0);

    AvmMainTask = (AvmTask){
        .__type = typeid(AvmTask),
        ._private =
            {
                .state = context,
                .previous = NULL,
                .stack = NULL,
            },
    };

    AvmCurrentTask = &AvmMainTask;
}

const AvmTask* AvmTaskNew(AvmTaskEntryPoint entry, object value)
{
    pre
    {
        assert(entry != NULL);
    }

    // This will not be collected because it is stored in the AvmTask class.
    void* stack = AvmAlloc(AVM_TASK_STACK_SIZE, true);

    // This should not contain pointers to GC memory right?
    coro_context* context = AvmAlloc(sizeof(coro_context), false);

    AvmTaskContext* tcontext = __AvmTaskContextNew(entry, value);

    // TODO: Maybe decrease size and add overflow protection.
    // Also TODO: Not reentrant.
    coro_create(context,
                (coro_func)AvmTaskForwarder,
                tcontext,
                stack,
                AVM_TASK_STACK_SIZE);

    AvmTask* task = AvmObjectNew(typeid(AvmTask));
    task->_private.stack = stack;
    task->_private.state = context;
    task->_private.previous = AvmCurrentTask;

    return task;
}

const AvmTask* AvmTaskGetMain()
{
    return &AvmMainTask;
}

const AvmTask* AvmTaskGetCurrent()
{
    return AvmCurrentTask;
}

void AvmTaskSwitchTo(const AvmTask* self)
{
    pre
    {
        assert(self != NULL);
    }

    const AvmTask* current = AvmCurrentTask;

    AvmCurrentTask = self;

    coro_transfer(current->_private.state, self->_private.state);
}

void AvmTaskSwitchBack()
{
    AvmTaskSwitchTo(AvmCurrentTask->_private.previous);
}
