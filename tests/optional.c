#include <avium/testing.h>
#include <avium/prologue.h>

AVM_OPTIONAL_TYPE(int)

static object TestInit() {
    static AvmOptional(int) optional;

    optional = AvmSome(int)(5);
    return &optional;
}

static void TestFini(object state) { (void)state; }

// TEST
static void TestHasValue(object state) { Assert(AvmHasValue(int)(state)); }

// TEST
static void TestGetValue(object state) {
    AssertEqual(AvmGetValue(int)(state), 5);
}
