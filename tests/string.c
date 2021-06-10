#include <avium/string.h>

#include <string.h> // For strcmp

static void TestFrom()
{
    AvmString s = AvmStringFrom("Hello");

    assert(AvmStringGetBuffer(&s) != NULL);
    assert(AvmStringGetLength(&s) == 5);
    assert(AvmStringGetCapacity(&s) == 5 * AVM_STRING_GROWTH_FACTOR);
    assert(strcmp(AvmStringGetBuffer(&s), "Hello") == 0);
    assert(AvmStringCharAt(&s, 1) == 'e');
    assert(!AvmStringIsEmpty(&s));
}

void Main()
{
    TestFrom();
}
