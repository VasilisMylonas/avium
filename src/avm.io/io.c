#include "avium/io.h"

#include "avium/private/resources.h"
#include "avium/string.h"

typedef AvmError* (*ReadWriteFunc)(AvmStream*, size_t, byte[]);

#define RESULT_CAST(result, TFrom, TTo)                             \
    AvmIsFailure(TFrom)(result) ? AvmFailure(TTo)((result)->_error) \
                                : AvmSuccess(TTo)((TTo)(result)->_value)

AvmError* AvmStreamFlush(AvmStream* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntryFlush];
    return ((AvmError * (*)(AvmStream*)) func)(self);
}

AvmError* AvmStreamSeek(AvmStream* self, _long offset, AvmSeekOrigin origin) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (origin == SeekOriginEnd && offset > 0) {
        AvmPanic(
            "Parameter `offset` must be smaller or equal to 0, if "
            "SeekOriginEnd is specified.");
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntrySeek];
    return ((AvmError * (*)(AvmStream*, _long, AvmSeekOrigin)) func)(
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

AvmError* AvmStreamRead(AvmStream* self, size_t length, byte buffer[]) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (buffer == NULL) {
        AvmPanic(BufferNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntryRead];
    return ((ReadWriteFunc)func)(self, length, buffer);
}

AvmError* AvmStreamWrite(AvmStream* self, size_t length, byte buffer[]) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (buffer == NULL) {
        AvmPanic(BufferNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FnEntryWrite];
    return ((ReadWriteFunc)func)(self, length, buffer);
}

byte AvmStreamReadByte(AvmStream* self, AvmError** error) {
    byte temp;
    AvmError* result = AvmStreamRead(self, 1, &temp);

    if (error != NULL) {
        *error = result;
    }

    return temp;
}

AvmError* AvmStreamWriteByte(AvmStream* self, byte value) {
    AvmStreamWrite(self, 1, &value);
    return NULL;
}

char AvmStreamReadChar(AvmStream* self, AvmError** error) {
    return (char)AvmStreamReadByte(self, error);
}

AvmError* AvmStreamWriteChar(AvmStream* self, char value) {
    return AvmStreamWriteByte(self, (byte)value);
}

AvmString AvmStreamReadLine(AvmStream* self, AvmError** error) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmString s = AvmStringNew(READ_LINE_CAPACITY);

    char c = AvmStreamReadChar(self, error);
    if (*error != NULL) {
        AvmObjectDestroy(&s);
        return AvmStringNew(0);
    }

    while (c != '\n') {
        AvmStringPushChar(&s, c);
        c = AvmStreamReadChar(self, error);
        if (*error != NULL) {
            AvmObjectDestroy(&s);
            return AvmStringNew(0);
        }
    }

    return s;
}

AvmError* AvmStreamWriteLine(AvmStream* self, AvmString* string) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (string == NULL) {
        AvmPanic(StringNullMsg);
    }

    AvmError* result = AvmStreamWrite(self, AvmStringGetLength(string),
                                      (byte*)AvmStringAsPtr(string));

    if (result != NULL) {
        return result;
    }

    return AvmStreamWriteChar(self, '\n');
}
