#include <avium/testing.h>
#include <avium/array.h>
#include <avium/fmt.h>

static object TestInit(int argc, str argv[]) {
    (void)argc;
    (void)argv;

    static AvmArray(int, 5) array;

    array = AvmArrayFrom(int, 5, 0, 1, -2, 3, -4);
    return &array;
}

__test TestArrayContents(object state) {
    AvmArray(int, 5)* array = state;
    int expected[] = {0, 1, -2, 3, -4};
    AssertMemEqual(array->at, expected, 5 * sizeof(int));
}

__test TestArrayToString(object state) {
    AvmArray(int, 5)* array = state;
    AvmString s = AvmObjectToString(array);
    AssertStrEqual(s._buffer, "[ 0, 1, -2, 3, -4, ]");
    AvmObjectDestroy(&s);
}
