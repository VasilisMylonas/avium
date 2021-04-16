#include "avium/io.h"

#include "avium/resources.h"
#include "avium/string.h"

typedef AvmResult(void) (*ReadWriteFunc)(AvmStream*, size_t, byte[]);

#define RESULT_CAST(result, TFrom, TTo)                             \
    AvmIsFailure(TFrom)(result) ? AvmFailure(TTo)((result)->_error) \
                                : AvmSuccess(TTo)((TTo)(result)->_value)

AvmResult(void) AvmStreamFlush(AvmStream* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntryRead];
    return ((AvmResult(void)(*)(AvmStream*))func)(self);
}

AvmResult(void)
    AvmStreamSeek(AvmStream* self, _long offset, AvmSeekOrigin origin) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (origin == SeekOriginEnd && offset > 0) {
        AvmPanic(
            "Parameter `offset` must be smaller or equal to 0, if "
            "SeekOriginEnd is specified.");
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntrySeek];
    return ((AvmResult(void)(*)(AvmStream*, _long, AvmSeekOrigin))func)(
        self, offset, origin);
}

size_t AvmStreamGetLength(AvmStream* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntryGetLength];
    return ((size_t(*)(AvmStream*))func)(self);
}

size_t AvmStreamGetPosition(AvmStream* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntryGetPosition];
    return ((size_t(*)(AvmStream*))func)(self);
}

AvmResult(void) AvmStreamRead(AvmStream* self, size_t length, byte buffer[]) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (buffer == NULL) {
        AvmPanic(BufferNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntryRead];
    return ((ReadWriteFunc)func)(self, length, buffer);
}

AvmResult(void) AvmStreamWrite(AvmStream* self, size_t length, byte buffer[]) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (buffer == NULL) {
        AvmPanic(BufferNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntryWrite];
    return ((ReadWriteFunc)func)(self, length, buffer);
}

AvmResult(byte) AvmStreamReadByte(AvmStream* self) {
    byte temp;
    AvmStreamRead(self, 1, &temp);
    return AvmSuccess(byte)(temp);
}

AvmResult(void) AvmStreamWriteByte(AvmStream* self, byte value) {
    AvmStreamWrite(self, 1, &value);
    return AvmSuccess(void)();
}

AvmResult(char) AvmStreamReadChar(AvmStream* self) {
    AvmResult(byte) result = AvmStreamReadByte(self);

    return RESULT_CAST(&result, byte, char);
}

AvmResult(void) AvmStreamWriteChar(AvmStream* self, char value) {
    return AvmStreamWriteByte(self, (byte)value);
}

AvmResult(AvmString) AvmStreamReadLine(AvmStream* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmString s = AvmStringNew(READ_LINE_CAPACITY);

    AvmResult(char) result = AvmStreamReadChar(self);
    if (AvmIsFailure(char)(&result)) {
        return AvmFailure(AvmString)(AvmErrorOfKind(ErrorKindRead));
    }
    char c = AvmUnwrap(char)(&result);

    while (c != '\n') {
        AvmStringPushChar(&s, c);
        result = AvmStreamReadChar(self);
        if (AvmIsFailure(char)(&result)) {
            return AvmFailure(AvmString)(AvmErrorOfKind(ErrorKindRead));
        }
        c = AvmUnwrap(char)(&result);
    }

    return AvmSuccess(AvmString)(s);
}

AvmResult(void) AvmStreamWriteLine(AvmStream* self, AvmString* string) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (string == NULL) {
        AvmPanic(StringNullMsg);
    }

    AvmResult(void) result = AvmStreamWrite(self, AvmStringGetLength(string),
                                            (byte*)AvmStringAsPtr(string));

    if (AvmIsFailure(void)(&result)) {
        return result;
    }

    return AvmStreamWriteChar(self, '\n');
}
