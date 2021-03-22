#include <avium/testing.h>
#include <avium/string.h>

static object TestInit() { return AvmStringFrom("Hello World Wordy World!"); }

static void TestFini(object state) { AvmObjectDestroy(state); }

// TEST
static void TestFrom(object state) {
    AssertNotNull(state);
    AssertEqual(AvmStringGetLength(state), 24);
    AssertEqual(AvmStringGetCapacity(state), AVM_STRING_GROWTH_FACTOR * 24);
}

// TEST
static void TestContents(object state) {
    AssertStringEqual(AvmStringAsPtr(state), "Hello World Wordy World!");
    AssertEqual(AvmStringCharAt(state, 2), 'l');
}

// TEST
static void TestIndexes(object state) {
    AssertEqual(AvmStringIndexOf(state, 'W'), 6);
    AssertEqual(AvmStringLastIndexOf(state, 'W'), 18);
    AssertEqual(AvmStringFind(state, "World"), 6);
    AssertEqual(AvmStringFindLast(state, "World"), 18);

    AssertEqual(AvmStringIndexOf(state, 'x'), AVM_STRING_NPOS);
    AssertEqual(AvmStringLastIndexOf(state, 'x'), AVM_STRING_NPOS);
    AssertEqual(AvmStringFind(state, "xxl"), AVM_STRING_NPOS);
    AssertEqual(AvmStringFindLast(state, "xxl"), AVM_STRING_NPOS);
}

// TEST
static void TestReplace(object state) {
    AssertEqual(AvmStringReplace(state, 'o', 'a'), 4);
    AssertStringEqual(AvmStringAsPtr(state), "Hella Warld Wardy Warld!");
}

// TEST
static void TestUpperLower(object state) {
    AvmStringToUpper(state);
    AssertStringEqual(AvmStringAsPtr(state), "HELLA WARLD WARDY WARLD!");
    AvmStringToLower(state);
    AssertStringEqual(AvmStringAsPtr(state), "hella warld wardy warld!");
}

// TEST
static void TestAppend(object state) {
    state = AvmStringAppend(state, ".com");
    AssertStringEqual(AvmStringAsPtr(state), "hella warld wardy warld!.com");
}
