#include <avium/string.h>
#include <avium/testing.h>

#include <string.h> // For strcmp

static void TestFrom()
{
    AvmString s = AvmStringFrom("Hello");

    assert_ne(AvmStringGetBuffer(&s), NULL);
    assert_eq(AvmStringGetLength(&s), 5);
    assert_eq(AvmStringGetCapacity(&s), 5 * AVM_STRING_GROWTH_FACTOR);
    assert_eq(strcmp(AvmStringGetBuffer(&s), "Hello"), 0);
    assert_eq(AvmStringCharAt(&s, 1), 'e');
    assert(!AvmStringIsEmpty(&s));
}

#include "avium/threads.h"

void ThreadProc(object o)
{
    (void)o;

    for (size_t i = 0; i < 20; i++)
    {
        AvmPrintf("Hello from 2\n");
        AvmThreadSleep(20);
    }
}

void Main()
{
    AvmThread thread = AvmThreadNew(ThreadProc, NULL);

    for (size_t i = 0; i < 20; i++)
    {
        AvmPrintf("Hello from 1\n");
        AvmThreadSleep(20);
    }

    AvmThreadJoin(&thread);

    TestFrom();
}
