#include "avium/task.h"

#include "avium/string.h"
#include "avium/thread.h"
#include "avium/typeinfo.h"

#include "avium/private/task-context.h"

#include <stdlib.h>

#include <deps/gc.h>
#include <deps/task.h>

static void AvmTaskFinalize(AvmTask* self)
{
    pre
    {
        assert(self != NULL);
    }

    (void)coro_destroy(self->_private.state);
}

static AvmString AvmTaskToString(const AvmTask* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(
        AVM_TASK_FMT_STR, self->_private.name, self->_private.id);
}

AVM_CLASS_TYPE(AvmTask,
               object,
               {
                   [AvmEntryFinalize] = (AvmCallback)AvmTaskFinalize,
                   [AvmEntryToString] = (AvmCallback)AvmTaskToString,
               });

static thread_local AvmTask* AvmCurrentTask = NULL;
static thread_local AvmTask AvmMainTask;
static AvmMutex AvmTaskMutex;
static uint AvmTaskID = 0;

AVM_CLASS_TYPE(AvmTaskContext, object, AVM_VTABLE_DEFAULT);

AvmTaskContext* __AvmTaskContextNew(AvmTaskEntryPoint entry, object value)
{
    AvmTaskContext* context = AvmObjectNew(typeid(AvmTaskContext));
    context->entry = entry;
    context->value = value;
    return context;
}

// This is used to automatically call AvmTaskExit on return.
static void AvmTaskForwarder(AvmTaskContext* context)
{
    context->entry(context->value);
    AvmTaskExit();
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
    AvmMutexLock(&AvmTaskMutex);
    coro_create(context, NULL, NULL, NULL, 0);

    AvmMainTask = (AvmTask){
        .__type = typeid(AvmTask),
        ._private =
            {
                .state = context,
                .previous = NULL,
                .stack = NULL,
                .name = AVM_TASK_MAIN_NAME,
                .retval = NULL,
                .id = AvmTaskID,
            },
    };
    AvmMutexUnlock(&AvmTaskMutex);

    AvmCurrentTask = &AvmMainTask;
}

AvmTask* AvmTaskNew(AvmTaskEntryPoint entry, object value)
{
    pre
    {
        assert(entry != NULL);
    }

    AvmTask* task =
        AvmTaskNewEx(entry, value, AVM_TASK_STACK_SIZE, AVM_TASK_DEFAULT_NAME);

    post
    {
        assert(task != NULL);
    }

    return task;
}

AvmTask* AvmTaskNewEx(AvmTaskEntryPoint entry,
                      object value,
                      uint stackSize,
                      str name)
{
    pre
    {
        assert(entry != NULL);
        assert(name != NULL);
        assert(stackSize > 4096);
        assert(stackSize % 16 == 0);
    }

    // This will not be collected because it is stored in the AvmTask class.
    void* stack = AvmAlloc(stackSize, true);

    // This should not contain pointers to GC memory right?
    coro_context* context = AvmAlloc(sizeof(coro_context), false);

    AvmTaskContext* tcontext = __AvmTaskContextNew(entry, value);

    AvmMutexLock(&AvmTaskMutex);
    // TODO: Maybe decrease size and add overflow protection.
    coro_create(context,
                (coro_func)AvmTaskForwarder,
                tcontext,
                stack,
                AVM_TASK_STACK_SIZE);

    // Increment the id.
    AvmTaskID++;

    AvmTask* task = AvmObjectNew(typeid(AvmTask));
    task->_private.id = AvmTaskID;
    task->_private.stack = stack;
    task->_private.state = context;
    task->_private.previous = AvmCurrentTask;
    task->_private.retval = NULL;
    task->_private.name = name;
    AvmMutexUnlock(&AvmTaskMutex);

    post
    {
        assert(task != NULL);
    }

    return task;
}

const AvmTask* AvmTaskGetMain(void)
{
    return &AvmMainTask;
}

const AvmTask* AvmTaskGetCurrent(void)
{
    post
    {
        assert(AvmCurrentTask != NULL);
    }

    return AvmCurrentTask;
}

str AvmTaskGetName(const AvmTask* self)
{
    pre
    {
        assert(self != NULL);
    }

    post
    {
        assert(self->_private.name != NULL);
    }

    return self->_private.name;
}

uint AvmTaskGetID(const AvmTask* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_private.id;
}

object AvmTaskSwitchTo(const AvmTask* self)
{
    pre
    {
        assert(self != NULL);
    }

    const AvmTask* current = AvmCurrentTask;

    AvmCurrentTask = (AvmTask*)self;

    coro_transfer(current->_private.state, self->_private.state);

    // Return the return value and clear it from the task.
    object o = self->_private.retval;
    ((AvmTask*)self)->_private.retval = NULL;
    return o;
}

static void AvmTaskReturnImpl(object value)
{

    if (AvmCurrentTask->_private.previous == NULL)
    {
        throw(AvmErrorNew("Cannot return from the main task."));
    }

    AvmCurrentTask->_private.retval = value;
    AvmTaskSwitchTo(AvmCurrentTask->_private.previous);
}

void AvmTaskSwitchBack(void)
{
    AvmTaskReturnImpl(NULL);
}

void AvmTaskReturn(object value)
{
    pre
    {
        assert(value != NULL);
        assert(value != AVM_TASK_EXITED);
    }

    AvmTaskReturnImpl(value);
}

never AvmTaskExit(void)
{
    AvmTaskReturnImpl(AVM_TASK_EXITED);

    // Impossible.
    abort();
}

object AvmTaskRun(const AvmTask* self)
{
    pre
    {
        assert(self != NULL);
    }

    object o = NULL;

    while (true)
    {
        object temp = AvmTaskSwitchTo(self);
        if (temp == AVM_TASK_EXITED)
        {
            break;
        }

        o = temp;
    }

    post
    {
        assert(o != AVM_TASK_EXITED);
    }

    return o;
}
