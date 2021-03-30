#include <avium/testing.h>
#include <avium/version.h>
#include <avium/string.h>

static object TestInit() {
    static AvmVersion version;
    version = AvmVersionFrom(0, 9, 1, 'b');
    return &version;
}

__test TestFields(object state) {
    AvmVersion* ver = (AvmVersion*)state;

    AssertEqual(ver->major, 0);
    AssertEqual(ver->minor, 9);
    AssertEqual(ver->patch, 1);
    AssertEqual(ver->tag, 'b');
}

__test TestToString(object state) {
    AvmString s = AvmObjectToString(state);
    AssertMemEqual(AvmStringAsPtr(&s), "0.9.1-b", AvmStringGetLength(state));
}

__test TestIsCompatible(object state) {
    AvmVersion version = AvmVersionFrom(0, 9, 25, 'r');
    Assert(AvmVersionIsCompatible(state, &version));

    version.major += 2;
    AssertNot(AvmVersionIsCompatible(state, &version));
}
