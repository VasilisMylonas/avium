#include "avium/thread.h"

#include "avium/typeinfo.h"

#include "avium/private/thread-context.h"

#include <deps/gc.h>
#include <stdlib.h>
#include <windows.h>

#define _ AvmRuntimeGetResource

void __AvmThreadContextSetThread(AvmThreadContext* self)
{
    HRESULT res = SetThreadDescription((HANDLE)state, self->_thread->_name);
    if (FAILED(res))
    {
        // TODO: We ignore this for now...
    }

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

    AvmThread* thread = __AvmThreadNewObject(name, false, stackSize);
    AvmThreadContext* context = __AvmThreadContextNew(value, entry, thread);

    void* h = (void*)_beginthreadex(
        NULL,
        stackSize,
        (unsigned (*)(void*))(AvmCallback)__AvmRuntimeThreadInit,
        context,
        0,
        NULL);

    if (h == NULL)
    {
        throw(AvmErrorNew(_(AvmThreadCreationErrorMsg)));
    }

    return __AvmThreadContextGetThread(context, h);
}

AvmExitCode AvmThreadJoin(AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    DWORD exitCode = 0;

    if (WaitForSingleObject((HANDLE)self->_state, INFINITE) == WAIT_FAILED)
    {
        throw(AvmErrorNew(_(AvmThreadJoinErrorMsg)));
    }

    lock(&self->_lock)
    {
        self->_isAlive = false;
    }

    if (GetExitCodeThread((HANDLE)self->_state, &exitCode) == 0)
    {
        throw(AvmErrorNew(_(AvmThreadJoinErrorMsg)));
    }

    return (AvmExitCode)exitCode;
}

void AvmThreadDetach(AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    if (CloseHandle((HANDLE)self->_state) == 0)
    {
        throw(AvmErrorNew(_(AvmThreadDetachErrorMsg)));
    }

    lock(&self->_lock)
    {
        self->_isDetached = true;
    }
}

void AvmThreadYield()
{
    Sleep(0);
}

never AvmThreadExit(AvmExitCode code)
{
    AvmThread* self = (AvmThread*)AvmThreadGetCurrent();

    lock(&self->_lock)
    {
        self->_isAlive = false;
    }

    _endthreadex((unsigned)code);
}

void AvmThreadTerminate(AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmObjectSurpressFinalizer(self);

    lock(&self->_lock)
    {
        self->_isAlive = false;
    }

    if (TerminateThread((HANDLE)self->_state, EXIT_FAILURE) == 0)
    {
        throw(AvmErrorNew(_(AvmArgErrorMsg)));
    }

    CloseHandle((HANDLE)self->_state);
}

AvmMutex AvmMutexNew(bool isRecursive)
{
    if (!isRecursive)
    {
        // TODO: Always recursive in win32...
    }

    LPCRITICAL_SECTION mutex = AvmAlloc(sizeof(CRITICAL_SECTION));
    InitializeCriticalSection(mutex);

    // We register a custom 'finalizer'.
    GC_register_finalizer(
        (void*)mutex,
        (GC_finalization_proc)(AvmCallback)DeleteCriticalSection,
        NULL,
        NULL,
        NULL);

    return (AvmMutex){
        ._type = typeid(AvmMutex),
        ._state = (void*)mutex,
    };
}

void AvmMutexLock(const AvmMutex* self)
{
    pre
    {
        assert(self != NULL);
    }

    EnterCriticalSection(self->_state);
}

void AvmMutexUnlock(const AvmMutex* self)
{
    pre
    {
        assert(self != NULL);
    }

    LeaveCriticalSection(self->_state);
}

AvmBarrier AvmBarrierNew(uint count)
{
    LPSYNCHRONIZATION_BARRIER barrier =
        AvmAlloc(sizeof(SYNCHRONIZATION_BARRIER));

    if (InitializeSynchronizationBarrier(barrier, count, -1) == FALSE)
    {
        throw(AvmErrorNew("TODO: Barrier failure."));
    }

    GC_register_finalizer(
        barrier,
        (GC_finalization_proc)(AvmCallback)DeleteSynchronizationBarrier,
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

    EnterSynchronizationBarrier(self->_state, 0);
}
