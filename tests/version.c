#include <avium/testing.h>
#include <avium/version.h>

static object TestInit() {
    static AvmVersion version;
    version = AvmVersionFrom(0, 9, 1, 'b');
    return &version;
}

static void TestFini(object state) { (void)state; }

// TEST
static void TestFields(object state) {
    AvmVersion* ver = (AvmVersion*)state;

    AssertEqual(ver->major, 0);
    AssertEqual(ver->minor, 9);
    AssertEqual(ver->patch, 1);
    AssertEqual(ver->tag, 'b');
}
