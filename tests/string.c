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

    AvmObjectDestroy(&s);
}

void main()
{
    TestFrom();
}
