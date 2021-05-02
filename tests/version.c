#include <avium/testing.h>
#include <avium/version.h>
#include <avium/string.h>

static object TestInit(int argc, str argv[]) {
    (void)argc;
    (void)argv;

    static AvmVersion version;
    version = AvmVersionFrom(0, 9, 1);
    return &version;
}

__test TestFields(object state) {
    AvmVersion* version = (AvmVersion*)state;

    AssertEqual(version->Major, 0);
    AssertEqual(version->Minor, 9);
    AssertEqual(version->Patch, 1);
}

__test TestToString(object state) {
    AvmString s = AvmObjectToString(state);
    AssertMemEqual(AvmStringGetBuffer(&s), "0.9.1", AvmStringGetLength(state));
}
