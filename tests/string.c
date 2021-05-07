#include <avium/testing.h>
#include <avium/string.h>

#include <string.h>

void TestNew(size_t capacity)
{
    const size_t length = 0;

    AvmString s = AvmStringNew(capacity);

    if (capacity == 0)
    {
        assert(AvmStringGetBuffer(&s) == NULL);
    }
    else
    {
        assert(AvmStringGetBuffer(&s) != NULL);
    }

    assert_eq(AvmStringGetLength(&s), length);
    assert_eq(AvmStringGetCapacity(&s), capacity);
    AvmObjectDestroy(&s);
}

void TestFrom(str string)
{
    const size_t length = strlen(string);

    AvmString s = AvmStringFrom(string);

    if (length == 0)
    {
        assert(AvmStringGetBuffer(&s) == NULL);
    }
    else
    {
        assert(AvmStringGetBuffer(&s) != NULL);
        assert_eq(strcmp(AvmStringGetBuffer(&s), string), 0);
    }

    assert_eq(AvmStringGetLength(&s), length);
    assert_eq(AvmStringGetCapacity(&s), length * AVM_STRING_GROWTH_FACTOR);
    AvmObjectDestroy(&s);
}

void TestFromChars(size_t length, char array[])
{
    AvmString s = AvmStringFromChars(length, array);

    if (length == 0)
    {
        assert(AvmStringGetBuffer(&s) == NULL);
    }
    else
    {
        assert(AvmStringGetBuffer(&s) != NULL);
    }

    assert_eq(AvmStringGetLength(&s), length);
    assert_eq(AvmStringGetCapacity(&s), length * AVM_STRING_GROWTH_FACTOR);
    AvmObjectDestroy(&s);
}

void TestRepeat(str string, size_t count)
{
    const size_t length = strlen(string);

    AvmString s = AvmStringRepeat(string, count);

    assert_eq(AvmStringGetLength(&s), count * length);
    assert_eq(AvmStringGetCapacity(&s),
              count * length * AVM_STRING_GROWTH_FACTOR);

    if (length == 0 || count == 0)
    {
        assert(AvmStringGetBuffer(&s) == NULL);
    }
    else
    {
        assert(AvmStringGetBuffer(&s) != NULL);
    }

    AvmObjectDestroy(&s);
}

void TestPush(str string)
{
    AvmString s = AvmStringFrom(string);
    AvmStringPushStr(&s, "!!");
}

int main()
{
    AvmEnableExceptions();

    for (size_t i = 0; i < 32; i++)
    {
        TestNew(i);
    }

    TestFrom("");
    TestFrom("Hello");

    TestFromChars(0, "");
    TestFromChars(0, "Hello");
    TestFromChars(5, "Hello");
    TestFromChars(3, "Hello");

    TestRepeat("", 0);
    TestRepeat("", 5);
    TestRepeat("Hello", 5);
    TestRepeat("Hello", 3);

    TestPush("Hello World");
}
