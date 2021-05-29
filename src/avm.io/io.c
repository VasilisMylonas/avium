#include "avium/io.h"

#include "avium/private/resources.h"
#include "avium/string.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

typedef AvmError* (*ReadWriteFunc)(AvmStream*, size_t, byte[]);

AvmError* AvmStreamFlush(AvmStream* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmFunction func = AvmObjectGetType(self)->_vPtr[FnEntryFlush];
    return ((AvmError * (*)(AvmStream*)) func)(self);
}

AvmError* AvmStreamSeek(AvmStream* self, _long offset, AvmSeekOrigin origin)
{
    pre
    {
        assert(self != NULL);
    }

    if (origin == SeekOriginEnd && offset > 0)
    {
        throw(
            AvmErrorNew("Parameter `offset` must be smaller or equal to 0, if "
                        "SeekOriginEnd is specified."));
    }

    AvmFunction func = AvmObjectGetType(self)->_vPtr[FnEntrySeek];
    return ((AvmError * (*)(AvmStream*, _long, AvmSeekOrigin))
                func)(self, offset, origin);
}

size_t AvmStreamGetLength(AvmStream* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmFunction func = AvmObjectGetType(self)->_vPtr[FnEntryGetLength];
    return ((size_t(*)(AvmStream*))func)(self);
}

size_t AvmStreamGetPosition(AvmStream* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmFunction func = AvmObjectGetType(self)->_vPtr[FnEntryGetPosition];
    return ((size_t(*)(AvmStream*))func)(self);
}

AvmError* AvmStreamRead(AvmStream* self, size_t length, byte buffer[])
{
    pre
    {
        assert(self != NULL);
        assert(buffer != NULL);
    }

    AvmFunction func = AvmObjectGetType(self)->_vPtr[FnEntryRead];
    return ((ReadWriteFunc)func)(self, length, buffer);
}

AvmError* AvmStreamWrite(AvmStream* self, size_t length, byte buffer[])
{
    pre
    {
        assert(self != NULL);
        assert(buffer != NULL);
    }

    AvmFunction func = AvmObjectGetType(self)->_vPtr[FnEntryWrite];
    return ((ReadWriteFunc)func)(self, length, buffer);
}

byte AvmStreamReadByte(AvmStream* self, AvmError** error)
{
    pre
    {
        assert(self != NULL);
    }

    byte temp;
    AvmError* result = AvmStreamRead(self, 1, &temp);

    if (error != NULL)
    {
        *error = result;
    }

    return temp;
}

AvmError* AvmStreamWriteByte(AvmStream* self, byte value)
{
    pre
    {
        assert(self != NULL);
    }

    AvmStreamWrite(self, 1, &value);
    return NULL;
}

char AvmStreamReadChar(AvmStream* self, AvmError** error)
{
    pre
    {
        assert(self != NULL);
    }

    return (char)AvmStreamReadByte(self, error);
}

AvmError* AvmStreamWriteChar(AvmStream* self, char value)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStreamWriteByte(self, (byte)value);
}

AvmString AvmStreamReadLine(AvmStream* self, AvmError** error)
{
    pre
    {
        assert(self != NULL);
    }

    AvmString s = AvmStringNew(READ_LINE_CAPACITY);

    char c = AvmStreamReadChar(self, error);
    if (*error != NULL)
    {
        return AvmStringNew(0);
    }

    while (c != '\n')
    {
        AvmStringPushChar(&s, c);
        c = AvmStreamReadChar(self, error);
        if (*error != NULL)
        {
            return AvmStringNew(0);
        }
    }

    return s;
}

AvmError* AvmStreamWriteLine(AvmStream* self, AvmString* string)
{
    pre
    {
        assert(self != NULL);
        assert(string != NULL);
    }

    AvmError* result = AvmStreamWrite(
        self, AvmStringGetLength(string), (byte*)AvmStringGetBuffer(string));

    if (result != NULL)
    {
        return result;
    }

    return AvmStreamWriteChar(self, '\n');
}
