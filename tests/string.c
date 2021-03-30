#include <avium/testing.h>
#include <avium/string.h>

static const str TestString = "Hello World!";
static const str TestString4 =
    "Hello World!Hello World!Hello World!Hello World!";
static const char TestArray[] = {'H', 'e', 'l', 'l', 'o', '!'};

static object TestInit() {
    static AvmString s;
    s = AvmStringFrom(TestString);
    return &s;
}

__test TestNew(object state) {
    (void)state;

    AvmString s;

    s = AvmStringNew(0);
    AssertNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), 0);
    AssertEqual(AvmStringGetCapacity(&s), 0);
    AvmObjectDestroy(&s);

    s = AvmStringNew(2);
    AssertNotNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), 0);
    AssertEqual(AvmStringGetCapacity(&s), 2);
    AvmObjectDestroy(&s);
}

__test TestFrom(object state) {
    (void)state;

    AvmString s;

    s = AvmStringFrom("");
    AssertNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), 0);
    AssertEqual(AvmStringGetCapacity(&s), 0);
    AvmObjectDestroy(&s);

    s = AvmStringFrom(TestString);
    AssertNotNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), 12);
    AssertEqual(AvmStringGetCapacity(&s), 12 * AVM_STRING_GROWTH_FACTOR);
    AssertStrEqual(AvmStringAsPtr(&s), TestString);
    AvmObjectDestroy(&s);
}

__test TestFromChars(object state) {
    (void)state;

    AvmString s;

    s = AvmStringFromChars(sizeof(TestArray), TestArray);
    AssertNotNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), sizeof(TestArray));
    AssertEqual(AvmStringGetCapacity(&s),
                sizeof(TestArray) * AVM_STRING_GROWTH_FACTOR);
    AssertMemEqual(AvmStringAsPtr(&s), "Hello!", sizeof(TestArray));
    AvmObjectDestroy(&s);

    s = AvmStringFromChars(3, TestArray);
    AssertNotNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), 3);
    AssertEqual(AvmStringGetCapacity(&s), 3 * AVM_STRING_GROWTH_FACTOR);
    AssertMemEqual(AvmStringAsPtr(&s), "Hel", 3);
    AvmObjectDestroy(&s);

    s = AvmStringFromChars(0, TestArray);
    AssertNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), 0);
    AssertEqual(AvmStringGetCapacity(&s), 0);
    AvmObjectDestroy(&s);
}

__test TestRepeat(object state) {
    (void)state;

    AvmString s;

    s = AvmStringRepeat(TestString, 4);
    AssertNotNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), 4 * strlen(TestString));
    AssertEqual(AvmStringGetCapacity(&s),
                4 * strlen(TestString) * AVM_STRING_GROWTH_FACTOR);
    AssertMemEqual(AvmStringAsPtr(&s), TestString4, 4 * strlen(TestString));
    AvmObjectDestroy(&s);

    s = AvmStringRepeat(TestString, 0);
    AssertNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), 0);
    AssertEqual(AvmStringGetCapacity(&s), 0);
    AvmObjectDestroy(&s);

    s = AvmStringRepeat("", 4);
    AssertNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), 0);
    AssertEqual(AvmStringGetCapacity(&s), 0);
    AvmObjectDestroy(&s);
}

#include <avium/fmt.h>

__test TestRepeatChars(object state) {
    (void)state;

    AvmString s;

    s = AvmStringRepeatChars(5, TestString, 4);
    AssertNotNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), 4 * 5);
    AssertEqual(AvmStringGetCapacity(&s), 4 * 5 * AVM_STRING_GROWTH_FACTOR);
    AssertMemEqual(AvmStringAsPtr(&s), "HelloHelloHelloHello", 4 * 5);
    AvmObjectDestroy(&s);

    s = AvmStringRepeatChars(5, TestString, 0);
    AssertNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), 0);
    AssertEqual(AvmStringGetCapacity(&s), 0);
    AvmObjectDestroy(&s);

    s = AvmStringRepeatChars(0, TestString, 4);
    AssertNull(AvmStringAsPtr(&s));
    AssertEqual(AvmStringGetLength(&s), 0);
    AssertEqual(AvmStringGetCapacity(&s), 0);
    AvmObjectDestroy(&s);
}

__test TestIndexes(object state) {
    (void)state;

    AvmOptional(size_t) index;
    AvmString s = AvmStringFrom("Hello Worldy World!");

    index = AvmStringIndexOf(&s, 'W');
    AssertEqual(AvmGetValue(size_t)(&index), 6);

    index = AvmStringLastIndexOf(&s, 'W');
    AssertEqual(AvmGetValue(size_t)(&index), 13);

    index = AvmStringFind(&s, "World");
    AssertEqual(AvmGetValue(size_t)(&index), 6);

    index = AvmStringFindLast(&s, "World");
    AssertEqual(AvmGetValue(size_t)(&index), 13);

    index = AvmStringIndexOf(&s, 'x');
    AssertNot(AvmHasValue(size_t)(&index));

    index = AvmStringLastIndexOf(&s, 'x');
    AssertNot(AvmHasValue(size_t)(&index));

    index = AvmStringFind(&s, "xxl");
    AssertNot(AvmHasValue(size_t)(&index));

    index = AvmStringFindLast(&s, "xxl");
    AssertNot(AvmHasValue(size_t)(&index));
}

__test TestPush(object state) {
    AvmStringPushStr(state, "!!");
    AssertStrEqual(AvmStringAsPtr(state), "Hello World!!!");
}

__test TestReplace(object state) {
    AssertEqual(AvmStringReplaceAll(state, 'o', 'a'), 2);
    AssertStrEqual(AvmStringAsPtr(state), "Hella Warld!");
}

__test TestUpperLower(object state) {
    AvmStringToUpper(state);
    AssertStrEqual(AvmStringAsPtr(state), "HELLO WORLD!");
    AvmStringToLower(state);
    AssertStrEqual(AvmStringAsPtr(state), "hello world!");
}
