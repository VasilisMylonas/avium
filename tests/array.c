#include <avium/testing.h>
#include <avium/array.h>
#include <avium/fmt.h>

static object TestInit() {
    static AvmArray(int, 5) array;

    array = AvmArrayFrom(int, 5)(0, 1, -2, 3, -4);
    return &array;
}

static void TestFini(object state) { (void)state; }

// TEST
static void TestArrayContents(object state) {
    AvmArray(int, 5)* array = state;
    int expected[] = {0, 1, -2, 3, -4};
    AssertMemEqual(array->at, expected, 5 * sizeof(int));
}

// TEST
static void TestArrayToString(object state) {
    AvmArray(int, 5)* array = state;
    AvmString s = AvmObjectToString(array);
    AssertStringEqual(s._buffer, "[ 0, 1, -2, 3, -4, ]");
    AvmObjectDestroy(&s);
}
