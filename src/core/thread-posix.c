#include "avium/thread.h"

#include "avium/typeinfo.h"

#include "avium/private/thread-context.h"

#include <pthread.h>

#include <deps/gc.h>

#if !defined AVM_HAVE_PTHREAD_BARRIER &&                                       \
    (!defined _POSIX_BARRIERS || _POSIX_BARRIERS <= 0)
#include <deps/pthread_barrier.h>
#endif

#define _ AvmRuntimeGetResource

void __AvmThreadContextSetThread(AvmThreadContext* self)
{
    // TODO: Temporarily disabled cause of Darwin.
    // #ifdef AVM_HAVE_PTHREAD_SETNAME
    // pthread_setname_np((pthread_t)state, self->_thread->_name);
    // #endif
    self->_thread->_isAlive = true;
}

AvmThread* AvmThreadNewEx(AvmThreadEntryPoint entry,
                          object value,
                          uint stackSize,
                          str name)
{
    pre
    {
        assert(entry != NULL);
        assert(name != NULL);
    }

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    if (stackSize != 0)
    {
        if (pthread_attr_setstacksize(&attr, stackSize) != 0)
        {
            throw(AvmErrorNew(_(AvmInvalidStackSizeErrorMsg)));
        }
    }

    AvmThread* thread = __AvmThreadNewObject(name, false, stackSize);
    AvmThreadContext* context = __AvmThreadContextNew(value, entry, thread);

    pthread_t id = 0;
    if (pthread_create(&id,
                       &attr,
                       (void* (*)(void*))(AvmCallback)__AvmRuntimeThreadInit,
                       context))
    {
        pthread_attr_destroy(&attr);
        throw(AvmErrorNew(_(AvmThreadCreationErrorMsg)));
    }

    pthread_attr_destroy(&attr);
    return __AvmThreadContextGetThread(context, (void*)id);
}

AvmExitCode AvmThreadJoin(AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    void* exitCode = NULL;

    if (pthread_join((pthread_t)self->_state, &exitCode) != 0)
    {
        throw(AvmErrorNew(_(AvmThreadJoinErrorMsg)));
    }

    AvmMutexLock(&self->_lock);
    self->_isAlive = false;
    AvmMutexUnlock(&self->_lock);

    return (AvmExitCode)(ulong)exitCode;
}

void AvmThreadDetach(AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    if (pthread_detach((pthread_t)self->_state) != 0)
    {
        throw(AvmErrorNew(_(AvmThreadDetachErrorMsg)));
    }

    AvmMutexLock(&self->_lock);
    self->_isDetached = true;
    AvmMutexUnlock(&self->_lock);
}

void AvmThreadYield()
{
    sched_yield();
}

never AvmThreadExit(AvmExitCode code)
{
    AvmThread* self = (AvmThread*)AvmThreadGetCurrent();

    AvmMutexLock(&self->_lock);
    self->_isAlive = false;
    AvmMutexUnlock(&self->_lock);

    pthread_exit((void*)(ulong)code);
}

void AvmThreadTerminate(AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmObjectSurpressFinalizer(self);

    AvmMutexLock(&self->_lock);
    self->_isAlive = false;
    AvmMutexUnlock(&self->_lock);

    if (pthread_cancel((pthread_t)self->_state) != 0)
    {
        throw(AvmErrorNew(_(AvmArgErrorMsg)));
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

    pthread_mutex_t* mutex = AvmAlloc(sizeof(pthread_mutex_t), false);
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
        .__type = typeid(AvmMutex),
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
    pthread_barrier_t* barrier = AvmAlloc(sizeof(pthread_barrier_t), false);
    pthread_barrier_init(barrier, NULL, count);
    GC_register_finalizer(
        barrier,
        (GC_finalization_proc)(AvmCallback)pthread_barrier_destroy,
        NULL,
        NULL,
        NULL);

    return (AvmBarrier){
        .__type = typeid(AvmBarrier),
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
