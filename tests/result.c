#include <avium/testing.h>
#include <avium/result.h>

static object TestInit() {
    static AvmResult(int) success;

    success = AvmSuccess(int)(5);
    return &success;
}

__test TestUnwrap(object state) { AssertEqual(AvmUnwrap(int)(state), 5); }

__test TestIsFailure(object state) { AssertNot(AvmIsFailure(int)(state)); }
