#include <avium/testing.h>
#include <avium/io.h>

#define STREAM_CAPACITY 512

static object TestInit(int argc, str argv[]) {
    (void)argc;
    (void)argv;

    return AvmStreamFromMemory(STREAM_CAPACITY);
}

__test TestGetLength(object state) {
    AssertEqual(AvmStreamGetLength(state), STREAM_CAPACITY);
}

__test TestGetPosition(object state) {
    AssertEqual(AvmStreamGetPosition(state), 0);
}

__test TestWriteByte(object state) {
    AvmStreamWriteByte(state, 255);

    AssertEqual(AvmStreamGetLength(state), STREAM_CAPACITY);
    AssertEqual(AvmStreamGetPosition(state), 1);
}

__test TestReadByte(object state) {
    AvmStreamWriteByte(state, 255);
    AvmStreamSeek(state, 0, SeekOriginBegin);

    AvmResult(byte) result = AvmStreamReadByte(state);
    AssertEqual(AvmUnwrap(byte)(&result), 255);
}

__test TestStreamSeek(object state) {
    AvmStreamSeek(state, 10, SeekOriginBegin);
    AssertEqual(AvmStreamGetPosition(state), 10);

    AvmStreamSeek(state, -10, SeekOriginEnd);
    AssertEqual(AvmStreamGetPosition(state), STREAM_CAPACITY - 10);

    AvmStreamSeek(state, 5, SeekOriginCurrent);
    AssertEqual(AvmStreamGetPosition(state), STREAM_CAPACITY - 10 + 5);

    AvmStreamSeek(state, 5, SeekOriginBegin);
    AssertEqual(AvmStreamGetPosition(state), 5);

    AvmStreamSeek(state, -10, SeekOriginBegin);
    AssertEqual(AvmStreamGetPosition(state), 5);
}

__test TestStreamFlush(object state) {
    AvmResult(void) result = AvmStreamFlush(state);
    AssertNot(AvmIsFailure(void)(&result));
}
