#include "avium/thread.h"

#include "avium/private/thread-context.h"

#include "avium/string.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

#include <pthread.h>
#include <setjmp.h>
#include <stdlib.h>
#include <unistd.h>

#include <bdwgc/include/gc.h>

AVM_TYPE(AvmThreadContext, object, {[FnEntryFinalize] = NULL});

AvmThreadContext* AvmThreadContextNew(object argument,
                                      AvmThreadEntryPoint entryPoint)
{
    pre
    {
        assert(entryPoint != NULL);
    }

    AvmThreadContext* t = AvmObjectNew(typeid(AvmThreadContext));
    t->_argument = argument;
    t->_entryPoint = entryPoint;
    t->_thread = NULL;
    return t;
}

AvmThread* AvmThreadContextGetThread(const AvmThreadContext* self)
{
    pre
    {
        assert(self != NULL);
    }

    while (self->_thread == NULL)
    {
        // Wait.
    }

    return self->_thread;
}

void AvmThreadContextEnter(const AvmThreadContext* self)
{
    pre
    {
        assert(self != NULL);
    }

    object obj = NULL;

    try
    {
        self->_entryPoint(self->_argument);
    }
    catch (object, e)
    {
        obj = e;
    }

    pthread_mutex_lock(self->_thread->_lock);
    self->_thread->_isAlive = false;
    pthread_mutex_unlock(self->_thread->_lock);

    if (obj != NULL)
    {
        throw(obj);
    }
}

AvmExitCode __AvmRuntimeThreadInit(AvmThreadContext* context)
{
    context->_thread = (AvmThread*)AvmThreadGetCurrent();

    AvmThrowContext c;
    __AvmRuntimePushThrowContext(&c);
    if (setjmp(c._jumpBuffer) == 0)
    {
        AvmThreadContextEnter(context);
    }
    else if (instanceof (object, c._thrownObject))
    {
        object e = __AvmRuntimePopThrowContext()->_thrownObject;
        AvmErrorf(
            AVM_UNHANDLED_THROW_FMT_STR, e, e, &c._location, context->_thread);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void AvmThreadFinalize(AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    pthread_mutex_lock(self->_lock);
    bool shouldTerminate = AvmThreadIsAlive(self) && !AvmThreadIsDetached(self);
    pthread_mutex_unlock(self->_lock);

    if (shouldTerminate)
    {
        AvmThreadTerminate(self);
    }

    pthread_mutex_destroy(self->_lock);
}

static AvmString AvmThreadToString(const AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(AVM_THREAD_FMT_STR, self->_name, self->_state);
}

AVM_TYPE(AvmThread,
         object,
         {
             [FnEntryToString] = (AvmCallback)AvmThreadToString,
             [FnEntryFinalize] = (AvmCallback)AvmThreadFinalize,
         });

AvmThread* AvmThreadNew(AvmThreadEntryPoint entry, object value)
{
    pre
    {
        assert(entry != NULL);
    }

    return AvmThreadNewEx(entry, value, 0, NULL, "<anonymous>");
}

AvmThread* AvmThreadNewEx(AvmThreadEntryPoint entry,
                          object value,
                          uint stackSize,
                          byte* stackPtr,
                          str name)
{
    pre
    {
        assert(entry != NULL);
    }

    AvmThreadContext* context = AvmThreadContextNew(value, entry);

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // TODO: These should be available through AvmThread.
    (void)name;
    if (stackSize != 0 && stackPtr != NULL)
    {
        pthread_attr_setstack(&attr, stackPtr, stackSize);
    }

    pthread_t id = 0;
    int result =
        GC_pthread_create(&id,
                          &attr,
                          (void* (*)(void*))(AvmCallback)__AvmRuntimeThreadInit,
                          context);

    pthread_attr_destroy(&attr);

    if (result != 0)
    {
        throw(AvmErrorNew("Thread creation failed."));
    }

    return AvmThreadContextGetThread(context);
}

bool AvmThreadIsDetached(const AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_isDetached;
}

bool AvmThreadIsAlive(const AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_isAlive;
}

str AvmThreadGetName(const AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_name;
}

uint AvmThreadGetCurrentID()
{
    return gettid();
}

const AvmThread* AvmThreadGetCurrent()
{
    static thread_local AvmThread* thread = NULL;

    if (thread == NULL)
    {
        thread = AvmObjectNew(typeid(AvmThread));
        thread->_lock = AvmAlloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(thread->_lock, NULL);

        // No need to lock here since the thread object isn't yet accessible.
        thread->_isAlive = true;
        thread->_isDetached = false;
        thread->_state = (void*)pthread_self();
    }

    return thread;
}

AvmExitCode AvmThreadJoin(AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    void* exitCode = NULL;

    if (GC_pthread_join((pthread_t)self->_state, &exitCode) != 0)
    {
        throw(AvmErrorNew("Could not join the requested thread."));
    }

    pthread_mutex_lock(self->_lock);
    self->_isAlive = false;
    pthread_mutex_unlock(self->_lock);

    return (AvmExitCode)(ulong)exitCode;
}

void AvmThreadDetach(AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    GC_pthread_detach((pthread_t)self->_state);

    pthread_mutex_lock(self->_lock);
    self->_isDetached = true;
    pthread_mutex_unlock(self->_lock);
}

void AvmThreadSleep(uint ms)
{
    usleep(ms * 1000);
}

void AvmThreadYield()
{
    sched_yield();
}

never AvmThreadExit(AvmExitCode code)
{
    // TODO: Possible future deinitialization code could be here.
    // Perhaps we could longjmp into __AvmRuntimeThreadInit and continue from
    // there?
    AvmThreadFastExit(code);
}

never AvmThreadFastExit(AvmExitCode code)
{
    AvmThread* self = (AvmThread*)AvmThreadGetCurrent();

    pthread_mutex_lock(self->_lock);
    self->_isAlive = false;
    pthread_mutex_unlock(self->_lock);

    GC_pthread_exit((void*)(ulong)code);
}

void AvmThreadTerminate(AvmThread* self)
{
    AvmObjectSurpressFinalizer(self);

    GC_pthread_cancel((pthread_t)self->_state);

    pthread_mutex_lock(self->_lock);
    self->_isAlive = false;
    pthread_mutex_unlock(self->_lock);
}
