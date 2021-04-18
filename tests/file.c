#include <avium/testing.h>
#include <avium/file.h>

static const str filename = "test.txt";
static byte contents[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

static object TestInit(int argc, str argv[]) {
    (void)argc;
    (void)argv;
    return NULL;
}

__test TestOpen(object state) {
    (void)state;

    AvmResult(AvmStreamPtr) result = AvmFileOpen(filename, FileAccessWrite);
    AvmStream* stream = AvmUnwrap(AvmStreamPtr)(&result);
    AvmStreamWrite(stream, sizeof(contents), contents);
    AvmObjectDestroy(stream);
}

__test TestReadAll(object state) {
    (void)state;

    byte values[sizeof(contents)];
    AvmFileReadAll(filename, sizeof(values), values);

    AssertMemEqual(contents, values, sizeof(contents));
}

__test TestExists(object state) {
    (void)state;

    Assert(AvmFileExists(filename));
}

__test TestDelete(object state) {
    (void)state;

    AvmFileDelete(filename);
    AssertNot(AvmFileExists(filename));
}
