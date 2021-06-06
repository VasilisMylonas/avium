#include "avium/threads.h"

#include "avium/private/threads.h"

#include "avium/testing.h"
#include "avium/typeinfo.h"

#include <errno.h>
#include <stdlib.h>

#include <bdwgc/include/gc.h>

#ifdef AVM_HAVE_POSIX_THREADS
#include <pthread.h>
#else
#include <process.h>
#endif

#ifdef AVM_WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

AvmThread AvmThreadNew(AvmThreadCallback callback, object value)
{
    pre
    {
        assert(callback != NULL);
    }

#ifdef AVM_HAVE_POSIX_THREADS
    pthread_t handle = 0;
    int result =
        GC_pthread_create(&handle,
                          NULL,
                          (void* (*)(void*))(AvmCallback)__AvmRuntimeThreadInit,
                          ThreadStateNew(value, callback));

    if (result != 0)
#else
    uintptr_t handle = GC_beginthreadex(
        NULL,
        0,
        (_beginthreadex_proc_type)(AvmCallback)__AvmRuntimeThreadInit,
        ThreadStateNew(value, callback),
        0,
        NULL);

    if (handle == NULL)
#endif
    {
        throw(AvmErrorNew("Thread creation failed."));
    }

    // Return the thread handle.
    return (AvmThread){
        ._type = typeid(AvmThread),
        ._state = (void*)handle,
        ._context = NULL,
    };
}

AvmExitCode AvmThreadJoin(const AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

#ifdef AVM_HAVE_POSIX_THREADS
    void* exitCode = NULL;

    if (GC_pthread_join((pthread_t)self->_state, &exitCode) != 0)
    {
        throw(AvmErrorNew("Could not join the requested thread."));
    }
#else
    DWORD exitCode = 0;

    if (WaitForSingleObject((HANDLE)self->_state, INFINITE) == WAIT_FAILED &&
        GetExitCodeThread((HANDLE)self->_state, &exitCode) == 0)
    {
        CloseHandle((HANDLE)self->_state);
    }
    else
    {
        CloseHandle((HANDLE)self->_state);
        throw(AvmErrorNew("Could not join the requested thread."));
    }
#endif

    return (AvmExitCode)(ulong)exitCode;
}

never AvmThreadExit(AvmExitCode code)
{
#ifdef AVM_HAVE_POSIX_THREADS
    GC_pthread_exit((void*)(ulong)code);
#else
    GC_endthreadex(code);
#endif
}

const AvmThread* AvmThreadGetCurrent(void)
{
    static thread_local AvmThread thread = {
        ._type = NULL,
        ._state = NULL,
        ._context = NULL,
    };

    if (thread._type == NULL)
    {
        thread._type = typeid(AvmThread);
#ifdef AVM_HAVE_POSIX_THREADS
        thread._state = (void*)pthread_self();
#else
        thread._state = (void*)GetCurrentThread();
#endif
    }

    return &thread;
}

void AvmThreadSleep(uint ms)
{
#ifdef AVM_WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

AVM_TYPE(AvmThread, object, {[FnEntryFinalize] = NULL});

#ifndef AVM_WIN32
AVM_TYPE(AvmMutex, object, {[FnEntryFinalize] = NULL});

AvmMutex AvmMutexNew()
{
    // TODO: Register finalizer.
    pthread_mutex_t* mutex = AvmAlloc(sizeof(pthread_mutex_t));

    int result = pthread_mutex_init(mutex, NULL);

    if (result != 0)
    {
        throw(AvmErrorFromOSCode(result));
    }

    return (AvmMutex){
        ._type = typeid(AvmMutex),
        ._mutex = mutex,
    };
}

void AvmMutexLock(const AvmMutex* self)
{
    pre
    {
        assert(self != NULL);
    }

    int result = pthread_mutex_lock(self->_mutex);

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

    int result = pthread_mutex_unlock(self->_mutex);

    if (result != 0)
    {
        throw(AvmErrorFromOSCode(result));
    }
}
#endif
