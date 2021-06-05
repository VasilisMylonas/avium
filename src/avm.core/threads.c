#define GC_THREADS
#include "gc.h"

#include "avium/threads.h"

#include "avium/private/constants.h"
#include "avium/private/threads.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

#include <errno.h>
#include <stdlib.h>

#ifdef AVM_WIN32

#include <process.h>
#include <windows.h>

AvmThread AvmThreadNew(AvmThreadCallback callback, object value)
{
    pre
    {
        assert(callback != NULL);
    }

    // Create a new thread.
    AvmThread thread = {
        ._type = typeid(AvmThread),
        ._state = (void*)GC_beginthreadex(
            NULL,
            0,
            (_beginthreadex_proc_type)__AvmRuntimeThreadInit,
            ThreadStateNew(value, callback),
            0,
            NULL),
    };

    // Handle errors and throw exceptions.
    if (thread._state == NULL)
    {
        if (errno == EACCES)
        {
            throw(AvmErrorNew(
                "The system could not allocate memory for the thread."));
        }
        throw(AvmErrorNew("Thread creation failed."));
    }

    // Return the thread handle.
    return thread;
}

AvmExitCode AvmThreadJoin(const AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    // Wait for the thread to finish executing.
    if (WaitForSingleObject((HANDLE)self->_state, INFINITE) == WAIT_FAILED)
    {
        CloseHandle((HANDLE)self->_state);
        throw(AvmErrorNew("Could not join the requested thread."));
    }

    // Get the thread return value.
    DWORD exitCode = 0;
    if (GetExitCodeThread((HANDLE)self->_state, &exitCode) == 0)
    {
        CloseHandle((HANDLE)self->_state);
        throw(AvmErrorNew("Could not join the requested thread."));
    }

    // Close the thread handle and return the return value.
    CloseHandle((HANDLE)self->_state);
    return (AvmExitCode)exitCode;
}

#else

#include <unistd.h>

AvmThread AvmThreadNew(AvmThreadCallback callback, object value)
{
    pre
    {
        assert(callback != NULL);
    }

    pthread_t t = 0;
    int result =
        GC_pthread_create(&t,
                          NULL,
                          (void* (*)(void*))(AvmCallback)__AvmRuntimeThreadInit,
                          ThreadStateNew(value, callback));

    if (result != 0)
    {
        throw(AvmErrorNew("Thread creation failed."));
    }

    // Return the thread handle.
    return (AvmThread){
        ._type = typeid(AvmThread),
        ._state = (void*)t,
        ._context = NULL,
    };
}

AvmExitCode AvmThreadJoin(const AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    // Try to join the thread and return the result.
    void* result = NULL;

    if (GC_pthread_join((pthread_t)self->_state, &result) != 0)
    {
        throw(AvmErrorNew("Could not join the requested thread."));
    }

    return (AvmExitCode)(ulong)result;
}

#endif

never AvmThreadExit(AvmExitCode code)
{
#ifdef AVM_WIN32
    GC_endthreadex(code);
#else
    GC_pthread_exit((void*)(ulong)code);
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
#ifdef AVM_WIN32
        thread._state = (void*)GetCurrentThread();
#else
        thread._state = (void*)pthread_self();
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
