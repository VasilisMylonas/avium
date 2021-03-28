#include <avium/testing.h>
#include <avium/prologue.h>

static object TestInit() {
    static AvmResult(int) success;

    success = AvmSuccess(int)(5);
    return &success;
}

static void TestFini(object state) { (void)state; }

// TEST
static void TestUnwrap(object state) { AssertEqual(AvmUnwrap(int)(state), 5); }

// TEST
static void TestIsFailure(object state) { AssertNot(AvmIsFailure(int)(state)); }
