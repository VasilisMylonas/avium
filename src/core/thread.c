#include "avium/thread.h"

#include "avium/string.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

#include "avium/private/thread-context.h"

#include <stdlib.h>

#ifdef AVM_WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static thread_local AvmThread* AvmCurrentThread = NULL;

AVM_TYPE(AvmThreadContext, object, {[FnEntryFinalize] = NULL});

AvmExitCode __AvmRuntimeThreadInit(AvmThreadContext* context)
{
    AvmCurrentThread = context->_thread;

    // At the same time AvmThreadContextGetThread will return.
    // Thread setup is now complete.
    __AvmThreadContextSetThread(context);

    // Time to handle uncaught thrown objects.
    AvmThrowContext c;
    __AvmRuntimePushThrowContext(&c);
    if (setjmp(c._jumpBuffer) == 0)
    {
        // Start executing the user code.
        __AvmThreadContextEnter(context);
    }
    else if (instanceof (object, c._thrownObject))
    {
        // We caught something!
        object e = __AvmRuntimePopThrowContext()->_thrownObject;
        AvmErrorf(
            AVM_UNHANDLED_THROW_FMT_STR, e, e, &c._location, context->_thread);
        return EXIT_FAILURE;
    }
    else
    {
        // We caught something that was not an instance of `object`. This should
        // never happen, but if it does it should be fatal. This should also
        // probably crash at instanceof since it makes some assumptions about
        // struct layout.
        abort();
    }

    return EXIT_SUCCESS;
}

AvmThread* __AvmThreadNewObject(str name,
                                bool isDetached,
                                uint stackSize,
                                byte* stackPtr)
{
    AvmThread* thread = AvmObjectNew(typeid(AvmThread));
    thread->_lock = AvmMutexNew();
    thread->_isAlive = true;
    thread->_isDetached = isDetached;
    thread->_name = name;
    thread->_stackPtr = stackPtr;
    thread->_stackSize = stackSize;
    return thread;
}

AvmThreadContext* __AvmThreadContextNew(object argument,
                                        AvmThreadEntryPoint entryPoint,
                                        AvmThread* thread)
{
    pre
    {
        assert(entryPoint != NULL);
    }

    AvmThreadContext* t = AvmObjectNew(typeid(AvmThreadContext));
    t->_argument = argument;
    t->_entryPoint = entryPoint;
    t->_thread = thread;
    return t;
}

void __AvmThreadContextEnter(const AvmThreadContext* self)
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

    if (obj != NULL)
    {
        throw(obj);
    }
}

AvmThread* __AvmThreadContextGetThread(const AvmThreadContext* self)
{
    pre
    {
        assert(self != NULL);
    }

    while (self->_thread->_state == NULL)
    {
        // Wait.
    }

    return self->_thread;
}

AvmThread* AvmThreadNew(AvmThreadEntryPoint entry, object value)
{
    pre
    {
        assert(entry != NULL);
    }

    return AvmThreadNewEx(entry, value, 0, NULL, AVM_THREAD_DEFAULT_NAME);
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

byte* AvmThreadGetStackPtr(const AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_stackPtr;
}

uint AvmThreadGetStackSize(const AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_stackSize;
}

const AvmThread* AvmThreadGetCurrent()
{
    return AvmCurrentThread;
}

void AvmThreadSleep(uint ms)
{
#ifdef AVM_WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

never AvmThreadExit(AvmExitCode code)
{
    // TODO: Possible future deinitialization code could be here.
    // Perhaps we could longjmp into __AvmRuntimeThreadInit and continue from
    // there?
    AvmThreadFastExit(code);
}

static void AvmThreadFinalize(AvmThread* self)
{
    pre
    {
        assert(self != NULL);
    }

    bool shouldTerminate;
    lock(&self->_lock)
    {
        shouldTerminate = AvmThreadIsAlive(self) && !AvmThreadIsDetached(self);
    }

    if (shouldTerminate)
    {
        AvmThreadTerminate(self);
    }
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

AVM_TYPE(AvmMutex, object, {[FnEntryFinalize] = NULL});
