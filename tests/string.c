#include <avium/testing.h>
#include <avium/string.h>

static object TestInit() {
    static AvmString s;
    s = AvmStringFrom("Hello World Wordy World!");
    return &s;
}

static void TestFini(object state) { (void)state; }

// TEST
static void TestFrom(object state) {
    AssertNotNull(state);
    AssertEqual(AvmStringGetLength(state), 24);
    AssertEqual(AvmStringGetCapacity(state), AVM_STRING_GROWTH_FACTOR * 24);
}

// TEST
static void TestContents(object state) {
    AssertStrEqual(AvmStringAsPtr(state), "Hello World Wordy World!");
    AvmResult(char) res = AvmStringCharAt(state, 2);
    AssertEqual(AvmUnwrap(char)(&res), 'l');
}

// TEST
static void TestIndexes(object state) {
    AvmOptional(size_t) index;

    index = AvmStringIndexOf(state, 'W');
    AssertEqual(AvmGetValue(size_t)(&index), 6);

    index = AvmStringLastIndexOf(state, 'W');
    AssertEqual(AvmGetValue(size_t)(&index), 18);

    index = AvmStringFind(state, "World");
    AssertEqual(AvmGetValue(size_t)(&index), 6);

    index = AvmStringFindLast(state, "World");
    AssertEqual(AvmGetValue(size_t)(&index), 18);

    index = AvmStringIndexOf(state, 'x');
    AssertNot(AvmHasValue(size_t)(&index));

    index = AvmStringLastIndexOf(state, 'x');
    AssertNot(AvmHasValue(size_t)(&index));

    index = AvmStringFind(state, "xxl");
    AssertNot(AvmHasValue(size_t)(&index));

    index = AvmStringFindLast(state, "xxl");
    AssertNot(AvmHasValue(size_t)(&index));
}

// TEST
static void TestReplace(object state) {
    AssertEqual(AvmStringReplaceAll(state, 'o', 'a'), 4);
    AssertStrEqual(AvmStringAsPtr(state), "Hella Warld Wardy Warld!");
}

// TEST
static void TestUpperLower(object state) {
    AvmStringToUpper(state);
    AssertStrEqual(AvmStringAsPtr(state), "HELLA WARLD WARDY WARLD!");
    AvmStringToLower(state);
    AssertStrEqual(AvmStringAsPtr(state), "hella warld wardy warld!");
}

// TEST
static void TestPush(object state) {
    AvmStringPushStr(state, ".com");
    AssertStrEqual(AvmStringAsPtr(state), "hella warld wardy warld!.com");
}
