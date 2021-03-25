#include <avium/testing.h>
#include <avium/array-list.h>

#include <stdlib.h>

AVM_ARRAY_LIST_TYPE(int)

static object TestInit() {
    static AvmArrayList(int) list;

    list = AvmArrayListNew(int)(16);
    return &list;
}

static void TestFini(object state) { (void)state; }

// TEST
static void TestGetCapacity(object state) {
    AssertEqual(AvmArrayListGetCapacity(int)(state), 16);
}

// TEST
static void TestGetLength(object state) {
    AssertEqual(AvmArrayListGetLength(int)(state), 0);
}

// TEST
static void TestPush(object state) {
    AvmArrayListPush(int)(state, 0);
    AvmArrayListPush(int)(state, 1);
    AvmArrayListPush(int)(state, 2);
    AvmArrayListPush(int)(state, 3);
    AvmArrayListPush(int)(state, 4);

    AssertEqual(AvmArrayListGetLength(int)(state), 5);
}

// TEST
static void TestPeek(object state) {
    AssertEqual(AvmArrayListPeek(int)(state), 4);
    AssertEqual(AvmArrayListGetLength(int)(state), 5);
}

// TEST
static void TestPop(object state) {
    AssertEqual(AvmArrayListPop(int)(state), 4);
    AssertEqual(AvmArrayListPop(int)(state), 3);
    AssertEqual(AvmArrayListPop(int)(state), 2);
    AssertEqual(AvmArrayListPop(int)(state), 1);
    AssertEqual(AvmArrayListPop(int)(state), 0);

    AssertEqual(AvmArrayListGetLength(int)(state), 0);
}
