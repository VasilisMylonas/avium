#ifndef AVIUM_IO_H
#define AVIUM_IO_H

#include "avium/runtime.h"
#include "avium/array-list.h"

typedef void* AvmFileHandle;

typedef enum {
    SeekOriginBegin,
    SeekOriginCurrent,
    SeekOriginEnd,
} AvmSeekOrigin;

AVM_CLASS(AvmStream, object, { size_t _position; });

AVM_CLASS(AvmFileStream, AvmStream, { AvmFileHandle _handle; });
AVM_CLASS(AvmMemoryStream, AvmStream, { AvmArrayList(byte) _list; });

static_assert_s(sizeof(AvmStream) == 16);
static_assert_s(sizeof(AvmFileStream) == 24);
static_assert_s(sizeof(AvmMemoryStream) == 48);

AVMAPI AvmStream* AvmStreamFromHandle(AvmFileHandle handle);
AVMAPI AvmStream* AvmStreamFromMemory(size_t capacity);
AVMAPI AvmStream* AvmStreamFromRaw(size_t length, byte array[]);

AVMAPI void AvmStreamRead(AvmStream* self, size_t length, byte bytes[]);
AVMAPI void AvmStreamWrite(AvmStream* self, size_t length, byte bytes[]);
AVMAPI void AvmStreamFlush(AvmStream* self);
AVMAPI void AvmStreamSeek(AvmStream* self, _long offset, AvmSeekOrigin origin);
AVMAPI size_t AvmStreamGetPosition(AvmStream* self);

AVMAPI byte AvmStreamReadByte(AvmStream* self);
AVMAPI void AvmStreamWriteByte(AvmStream* self, byte value);

AVMAPI char AvmStreamReadChar(AvmStream* self);
AVMAPI void AvmStreamWriteChar(AvmStream* self, char character);

AVMAPI AvmString AvmStreamReadLine(AvmStream* self);
AVMAPI void AvmStreamWriteLine(AvmStream* self, AvmString* string);

#endif  // AVIUM_IO_H
