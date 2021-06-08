#include "avium/thread.h"

#include "avium/testing.h"
#include "avium/typeinfo.h"

void ThreadProc(object value)
{
    (void)value;

    while (true)
    {
        AvmThreadSleep(100);
        AvmPrintf("Working!\n");
    }
}

// typedef enum
// {
//     AvmThreadPriorityDefault,
//     AvmThreadPriorityLow,
//     AvmThreadPriorityMedium,
//     AvmThreadPriorityHigh,
// } AvmThreadPriority;

// AVMAPI AvmThreadPriority AvmThreadGetPriority(const AvmThread* self);

void Main()
{
    AvmThread* thread = AvmThreadNew(ThreadProc, NULL);

    assert(AvmThreadIsAlive(thread));
    assert(!AvmThreadIsDetached(thread));

    AvmThreadDetach(thread);

    AvmPrintf("%v\n", typeid(AvmThread));

    assert(AvmThreadIsAlive(thread));
    assert(AvmThreadIsDetached(thread));

    AvmThreadSleep(500);
    AvmThreadTerminate(thread);

    assert(!AvmThreadIsAlive(thread));
}
