#include "avium/thread.h"

#include "avium/testing.h"
#include "avium/typeinfo.h"

#include "avium/private/thread-context.h"

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <bdwgc/include/gc.h>

static thread_local AvmThread* AvmCurrentThread = NULL;

// This is the first thing that runs in a newly created thread.
AvmExitCode __AvmRuntimeThreadInit(AvmThreadContext* context)
{
    AvmCurrentThread = context->_thread;

    // This can only be done from this thread.
    // At the same time AvmThreadContextGetThread will return from the other
    // thread.
    AvmCurrentThread->_state = (void*)pthread_self();

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

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    if (stackSize != 0 && stackPtr != NULL)
    {
        pthread_attr_setstack(&attr, stackPtr, stackSize);
    }
    else
    {
        size_t stackSize2 = 0;
        void* stackPtr2 = NULL;
        pthread_attr_getstack(&attr, &stackPtr2, &stackSize2);
        stackSize = (uint)stackSize2;
        stackPtr = (byte*)stackPtr2;
    }

    AvmThread* thread =
        __AvmRuntimeCreateThreadObject(name, false, stackSize, stackPtr);

    AvmThreadContext* context = AvmThreadContextNew(value, entry, thread);

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

const AvmThread* AvmThreadGetCurrent()
{
    return AvmCurrentThread;
}

uint AvmThreadGetCurrentID()
{
    return gettid();
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

    lock(&self->_lock)
    {
        self->_isAlive = false;
    }

    return (AvmExitCode)(ulong)exitCode;
}

void AvmThreadDetach(AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    GC_pthread_detach((pthread_t)self->_state);

    lock(&self->_lock)
    {
        self->_isDetached = true;
    }
}

void AvmThreadSleep(uint ms)
{
    usleep(ms * 1000);
}

void AvmThreadYield()
{
    sched_yield();
}

never AvmThreadFastExit(AvmExitCode code)
{
    AvmThread* self = (AvmThread*)AvmThreadGetCurrent();

    lock(&self->_lock)
    {
        self->_isAlive = false;
    }

    GC_pthread_exit((void*)(ulong)code);
}

void AvmThreadTerminate(AvmThread* self)
{
    AvmObjectSurpressFinalizer(self);

    GC_pthread_cancel((pthread_t)self->_state);

    lock(&self->_lock)
    {
        self->_isAlive = false;
    }
}

AvmMutex AvmMutexNew()
{
    // TODO: Register finalizer.
    pthread_mutex_t* mutex = AvmAlloc(sizeof(pthread_mutex_t));

    // We register a custom 'finalizer'.
    GC_register_finalizer(
        mutex,
        (GC_finalization_proc)(AvmCallback)pthread_mutex_destroy,
        NULL,
        NULL,
        NULL);

    int result = pthread_mutex_init(mutex, NULL);

    if (result != 0)
    {
        throw(AvmErrorFromOSCode(result));
    }

    return (AvmMutex){
        ._type = typeid(AvmMutex),
        ._state = mutex,
    };
}

void AvmMutexLock(const AvmMutex* self)
{
    pre
    {
        assert(self != NULL);
    }

    int result = pthread_mutex_lock(self->_state);

    if (result != 0)
    {
        throw(AvmErrorFromOSCode(result));
    }
}

void AvmMutexUnlock(const AvmMutex* self)
{
    pre
    {
        assert(self != NULL);
    }

    int result = pthread_mutex_unlock(self->_state);

    if (result != 0)
    {
        throw(AvmErrorFromOSCode(result));
    }
}
