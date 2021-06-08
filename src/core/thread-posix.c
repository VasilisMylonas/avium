#include "avium/thread.h"

#include "avium/testing.h"
#include "avium/typeinfo.h"

#include "avium/private/thread-context.h"

#include <pthread.h>
#include <unistd.h>

#include <bdwgc/include/gc.h>

void __AvmThreadContextSetThread(AvmThreadContext* self)
{
    void* state = (void*)pthread_self();

    // This would be bad.
    assert(state != NULL);

#ifdef AVM_HAVE_PTHREAD_SETNAME
    pthread_setname_np((pthread_t)state, name);
#endif

    self->_thread->_state = state;
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
        assert(name != NULL);
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

    AvmThread* thread = __AvmThreadNewObject(name, false, stackSize, stackPtr);

    AvmThreadContext* context = __AvmThreadContextNew(value, entry, thread);

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

    return __AvmThreadContextGetThread(context);
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
    pre
    {
        assert(self != NULL);
    }

    AvmObjectSurpressFinalizer(self);

    GC_pthread_cancel((pthread_t)self->_state);

    lock(&self->_lock)
    {
        self->_isAlive = false;
    }
}

AvmMutex AvmMutexNew(bool isRecursive)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);

    if (isRecursive)
    {
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    }

    pthread_mutex_t* mutex = AvmAlloc(sizeof(pthread_mutex_t));
    int result = pthread_mutex_init(mutex, &attr);

    pthread_mutexattr_destroy(&attr);

    if (result != 0)
    {
        throw(AvmErrorFromOSCode(result));
    }

    // We register a custom 'finalizer'.
    GC_register_finalizer(
        mutex,
        (GC_finalization_proc)(AvmCallback)pthread_mutex_destroy,
        NULL,
        NULL,
        NULL);

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

AvmBarrier AvmBarrierNew(uint count)
{
    pthread_barrier_t* barrier = AvmAlloc(sizeof(pthread_barrier_t));
    pthread_barrier_init(barrier, NULL, count);
    GC_register_finalizer(
        barrier,
        (GC_finalization_proc)(AvmCallback)pthread_barrier_destroy,
        NULL,
        NULL,
        NULL);

    return (AvmBarrier){
        ._type = typeid(AvmBarrier),
        ._state = barrier,
    };
}

void AvmBarrierWait(const AvmBarrier* self)
{
    pre
    {
        assert(self != NULL);
    }

    pthread_barrier_wait(self->_state);
}
