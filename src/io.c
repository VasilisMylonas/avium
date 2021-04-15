#include "avium/io.h"
#include "avium/resources.h"
#include "avium/string.h"

typedef AvmResult(void) (*ReadWriteFunc)(AvmStream*, size_t, byte[]);

#define RESULT_CAST(result, TFrom, TTo)                             \
    AvmIsFailure(TFrom)(result) ? AvmFailure(TTo)((result)->_error) \
                                : AvmSuccess(TTo)((TTo)(result)->_value)

void AvmStreamFlush(AvmStream* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FUNC_READ];
    ((void (*)(AvmStream*))func)(self);
}

void AvmStreamSeek(AvmStream* self, _long offset, AvmSeekOrigin origin) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FUNC_READ];
    ((void (*)(AvmStream*, _long, AvmSeekOrigin))func)(self, offset, origin);
}

size_t AvmStreamGetPosition(AvmStream* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FUNC_GET_POSITION];
    return ((ulong(*)(AvmStream*))func)(self);
}

AvmResult(void) AvmStreamRead(AvmStream* self, size_t length, byte bytes[]) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FUNC_READ];
    return ((ReadWriteFunc)func)(self, length, bytes);
}

AvmResult(void) AvmStreamWrite(AvmStream* self, size_t length, byte bytes[]) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FUNC_WRITE];
    return ((ReadWriteFunc)func)(self, length, bytes);
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

AvmResult(void) AvmStreamWriteChar(AvmStream* self, char character) {
    return AvmStreamWriteByte(self, (byte)character);
}

AvmResult(void) AvmStreamWriteLine(AvmStream* self, AvmString* string) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self == NULL) {
        AvmPanic(StringNullMsg);
    }

    AvmStreamWrite(self, AvmStringGetLength(string),
                   (byte*)AvmStringAsPtr(string));
    return AvmStreamWriteChar(self, '\n');
}

AvmResult(AvmString) AvmStreamReadLine(AvmStream* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmString s = AvmStringNew(32);
    char c;
    AvmResult(char) result;

    result = AvmStreamReadChar(self);
    if (AvmIsFailure(char)(&result)) {
        // TODO
        AvmPanic("Could not read line.");
        // return AvmFailure(AvmString)(ErrorKindSys, "Could not read line.");
    }
    c = AvmUnwrap(char)(&result);

    while (c != '\n') {
        AvmStringPushChar(&s, c);
        result = AvmStreamReadChar(self);
        if (AvmIsFailure(char)(&result)) {
            // TODO
            AvmPanic("Could not read line.");
            // return AvmFailure(AvmString)(ErrorKindSys, "Could not read
            // line.");
        }
        c = AvmUnwrap(char)(&result);
    }

    return AvmSuccess(AvmString)(s);
}
