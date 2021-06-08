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

void Main()
{
    AvmThread* thread = AvmThreadNew(ThreadProc, NULL);

    assert(AvmThreadIsAlive(thread));
    assert(!AvmThreadIsDetached(thread));

    AvmThreadDetach(thread);

    assert(AvmThreadIsAlive(thread));
    assert(AvmThreadIsDetached(thread));

    AvmThreadSleep(500);
    AvmThreadTerminate(thread);

    assert(!AvmThreadIsAlive(thread));
}
