#include "avium/io.h"
#include "avium/resources.h"
#include "avium/string.h"

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

void AvmStreamRead(AvmStream* self, size_t length, byte bytes[]) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FUNC_READ];
    ((void (*)(AvmStream*, size_t, byte[]))func)(self, length, bytes);
}

void AvmStreamWrite(AvmStream* self, size_t length, byte bytes[]) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmFunction func = AvmObjectGetType(self)->_vptr[FUNC_WRITE];
    ((void (*)(AvmStream*, size_t, byte[]))func)(self, length, bytes);
}

byte AvmStreamReadByte(AvmStream* self) {
    byte temp;
    AvmStreamRead(self, 1, &temp);
    return temp;
}

void AvmStreamWriteByte(AvmStream* self, byte value) {
    AvmStreamWrite(self, 1, &value);
}

char AvmStreamReadChar(AvmStream* self) {
    return (char)AvmStreamReadByte(self);
}

void AvmStreamWriteChar(AvmStream* self, char character) {
    AvmStreamWriteByte(self, (byte)character);
}

void AvmStreamWriteLine(AvmStream* self, AvmString* string) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self == NULL) {
        AvmPanic(StringNullMsg);
    }

    AvmStreamWrite(self, AvmStringGetLength(string),
                   (byte*)AvmStringAsPtr(string));
    AvmStreamWrite(self, 1, (byte*)"\n");
}

AvmString AvmStreamReadLine(AvmStream* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmString s = AvmStringNew(32);

    char c = '\0';
    while (AvmStreamRead(self, 1, (byte*)&c), c != '\n') {
        AvmStringPushChar(&s, c);
    }

    return s;
}
