#include <avium/testing.h>
#include <avium/array-list.h>

static object TestInit() {
    static AvmArrayList(int) list;

    list = AvmArrayListNew(int)(16);
    return &list;
}

__test TestFini(object state) { (void)state; }

__test TestGetCapacity(object state) {
    AssertEqual(AvmArrayListGetCapacity(int)(state), 16);
}

__test TestGetLength(object state) {
    AssertEqual(AvmArrayListGetLength(int)(state), 0);
}

__test TestPush(object state) {
    AvmArrayListPush(int)(state, 0);
    AvmArrayListPush(int)(state, 1);
    AvmArrayListPush(int)(state, 2);
    AvmArrayListPush(int)(state, 3);
    AvmArrayListPush(int)(state, 4);

    AssertEqual(AvmArrayListGetLength(int)(state), 5);
}

__test TestPeek(object state) {
    AssertEqual(AvmArrayListPeek(int)(state), 4);
    AssertEqual(AvmArrayListGetLength(int)(state), 5);
}

__test TestPop(object state) {
    AssertEqual(AvmArrayListPop(int)(state), 4);
    AssertEqual(AvmArrayListPop(int)(state), 3);
    AssertEqual(AvmArrayListPop(int)(state), 2);
    AssertEqual(AvmArrayListPop(int)(state), 1);
    AssertEqual(AvmArrayListPop(int)(state), 0);

    AssertEqual(AvmArrayListGetLength(int)(state), 0);
}
