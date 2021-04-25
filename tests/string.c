#include <avium/testing.h>
#include <avium/string.h>

static const str TestString = "Hello World!";
static const str TestString4 =
    "Hello World!Hello World!Hello World!Hello World!";
static const char TestArray[] = {'H', 'e', 'l', 'l', 'o', '!'};

static object TestInit(int argc, str argv[]) {
    (void)argc;
    (void)argv;

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

    AvmString s = AvmStringFrom("Hello Worldy World!");

    AssertEqual(AvmStringIndexOf(&s, 'W'), 6);
    AssertEqual(AvmStringLastIndexOf(&s, 'W'), 13);
    AssertEqual(AvmStringFind(&s, "World"), 6);
    AssertEqual(AvmStringFindLast(&s, "World"), 13);
    AssertEqual(AvmStringIndexOf(&s, 'x'), AvmInvalid);
    AssertEqual(AvmStringLastIndexOf(&s, 'x'), AvmInvalid);
    AssertEqual(AvmStringFind(&s, "xxl"), AvmInvalid);
    AssertEqual(AvmStringFindLast(&s, "xxl"), AvmInvalid);
}

__test TestPush(object state) {
    AvmStringPushStr(state, "!!");
    AssertStrEqual(AvmStringAsPtr(state), "Hello World!!!");
}

__test TestReplaceAll(object state) {
    AssertEqual(AvmStringReplaceAll(state, 'o', 'a'), 2);
    AssertStrEqual(AvmStringAsPtr(state), "Hella Warld!");
}

__test TestUpperLower(object state) {
    AvmStringToUpper(state);
    AssertStrEqual(AvmStringAsPtr(state), "HELLO WORLD!");
    AvmStringToLower(state);
    AssertStrEqual(AvmStringAsPtr(state), "hello world!");
}

__test TestReplaceN(object state) {
    AssertEqual(AvmStringReplaceN(state, 1, 'o', 'a'), 1);
    AssertStrEqual(AvmStringAsPtr(state), "Hella World!");
}

__test TestReplaceLastN(object state) {
    AssertEqual(AvmStringReplaceLastN(state, 1, 'o', 'a'), 1);
    AssertStrEqual(AvmStringAsPtr(state), "Hello Warld!");
}

__test TestReplace(object state) {
    AssertEqual(AvmStringReplace(state, 'o', 'a'), 4);
    AssertStrEqual(AvmStringAsPtr(state), "Hella World!");

    AssertEqual(AvmStringReplace(state, 'u', 'a'), AvmInvalid);
    AssertStrEqual(AvmStringAsPtr(state), "Hella World!");
}

__test TestReplaceLast(object state) {
    AssertEqual(AvmStringReplaceLast(state, 'o', 'a'), 7);
    AssertStrEqual(AvmStringAsPtr(state), "Hello Warld!");

    AssertEqual(AvmStringReplaceLast(state, 'u', 'a'), AvmInvalid);
    AssertStrEqual(AvmStringAsPtr(state), "Hello Warld!");
}

__test TestStartsWithStr(object state) {
    Assert(AvmStringStartsWithStr(state, "Hello"));
    AssertNot(AvmStringStartsWithStr(state, "Hella"));
}

__test TestStartsWithChar(object state) {
    Assert(AvmStringStartsWithChar(state, 'H'));
    AssertNot(AvmStringStartsWithChar(state, 'e'));
}

__test TestEndsWithStr(object state) {
    Assert(AvmStringEndsWithStr(state, "World!"));
    AssertNot(AvmStringEndsWithStr(state, "Hello"));
}

__test TestEndsWithChar(object state) {
    Assert(AvmStringEndsWithChar(state, '!'));
    AssertNot(AvmStringEndsWithChar(state, 'H'));
}

__test TestContainsStr(object state) {
    Assert(AvmStringContainsStr(state, "ello W"));
    AssertNot(AvmStringContainsStr(state, "ello X"));
}

__test TestContainsChar(object state) {
    Assert(AvmStringContainsChar(state, 'e'));
    AssertNot(AvmStringContainsChar(state, 'X'));
}

__test TestReverse(object state) {
    AvmStringReverse(state);
    AssertMemEqual(AvmStringAsPtr(state), "!dlroW olleH",
                   AvmStringGetLength(state));
}

__test TestClear(object state) {
    AvmStringClear(state);
    AssertEqual(AvmStringGetLength(state), 0);
}

__test TestErase(object state) {
    byte desired[12] = {0};
    AvmStringErase(state);
    AssertMemEqual(AvmStringAsPtr(state), desired, sizeof(desired));
    AssertEqual(AvmStringGetLength(state), 0);
}

__test TestUnsafeSetLength(object state) {
    AvmStringUnsafeSetLength(state, 5360);
    AssertEqual(AvmStringGetLength(state), 5360);
}

__test TestCharAt(object state) {
    AssertEqual(AvmStringCharAt(state, 0, NULL), 'H');
    AssertEqual(AvmStringCharAt(state, 100, NULL), '\0');
}

static size_t timesCalled = 0;
static size_t finalIndex = 0;

static char MapStub(char c) {
    timesCalled++;
    return c;
}

static char MapExStub(char c, size_t index) {
    timesCalled++;
    finalIndex = index;
    return c;
}

__test TestMap(object state) {
    size_t length = AvmStringGetLength(state);
    AvmStringMap(state, MapStub);
    AssertEqual(length, timesCalled);
    timesCalled = 0;
}

__test TestMapEx(object state) {
    size_t length = AvmStringGetLength(state);
    AvmStringMapEx(state, MapExStub);
    AssertEqual(length, timesCalled);
    AssertEqual(length - 1, finalIndex);
    timesCalled = 0;
    finalIndex = 0;
}

__test TestIsEmpty(object state) {
    AssertNot(AvmStringIsEmpty(state));

    AvmString s = AvmStringNew(0);
    Assert(AvmStringIsEmpty(&s));
}

__test TestToString(object state) {
    AvmString s = AvmObjectToString(state);
    AssertEqual(AvmStringGetLength(state), AvmStringGetLength(&s));
    AssertMemEqual(AvmStringAsPtr(state), AvmStringAsPtr(&s),
                   AvmStringGetLength(state));
}

__test TestClone(object state) {
    AvmString* s = AvmObjectClone(state);
    AssertEqual(AvmStringGetLength(state), AvmStringGetLength(s));
    AssertEqual(AvmStringGetLength(state), AvmStringGetLength(s));
    AssertMemEqual(AvmStringAsPtr(state), AvmStringAsPtr(s),
                   AvmStringGetLength(state));
}

__test TestUnsafeDestruct(object state) {
    size_t capacity;
    size_t length;
    char* buffer;

    AvmStringUnsafeDestruct(state, &capacity, &length, &buffer);

    AssertEqual(capacity, AvmStringGetCapacity(state));
    AssertEqual(length, AvmStringGetLength(state));
    AssertEqual(buffer, AvmStringAsPtr(state));
}
