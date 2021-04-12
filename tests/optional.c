#include <avium/testing.h>
#include <avium/optional.h>

static object TestInit(int argc, str argv[]) {
    (void)argc;
    (void)argv;

    static AvmOptional(int) optional;

    optional = AvmSome(int)(5);
    return &optional;
}

__test TestHasValue(object state) { Assert(AvmHasValue(int)(state)); }
__test TestGetValue(object state) { AssertEqual(AvmGetValue(int)(state), 5); }
