#ifndef AVIUM_IO_H
#define AVIUM_IO_H

#include "avium/runtime.h"
#include "avium/result.h"

typedef void* AvmFileHandle;

/// Defines the origin point for a seek operation.
typedef enum {
    /// Seek from the beginning of the stream.
    SeekOriginBegin = 0,

    /// Seek from the current stream position.
    SeekOriginCurrent,

    /// Seek backwards from the end of the stream.
    SeekOriginEnd,
} AvmSeekOrigin;

/// An abstraction over a sequence of bytes.
AVM_INTERFACE(AvmStream);

/**
 * @brief Creates a stream from a file handle.
 *
 * @pre Parameter @p handle must be not null.
 *
 * @param handle The file handle.
 * @return The created stream.
 */
AVMAPI AvmStream* AvmStreamFromHandle(AvmFileHandle handle);

/**
 * @brief Creates a stream from heap memory.
 *
 * @param capacity The initial capacity of the stream.
 * @return The created stream.
 */
AVMAPI AvmStream* AvmStreamFromMemory(size_t capacity);

// TODO
AVMAPI AvmStream* AvmStreamFromRaw(size_t length, byte array[]);

AVMAPI AvmResult(void)
    AvmStreamRead(AvmStream* self, size_t length, byte bytes[]);

AVMAPI AvmResult(void)
    AvmStreamWrite(AvmStream* self, size_t length, byte bytes[]);

AVMAPI void AvmStreamFlush(AvmStream* self);
AVMAPI void AvmStreamSeek(AvmStream* self, _long offset, AvmSeekOrigin origin);
AVMAPI size_t AvmStreamGetPosition(AvmStream* self);

AVMAPI AvmResult(byte) AvmStreamReadByte(AvmStream* self);
AVMAPI AvmResult(void) AvmStreamWriteByte(AvmStream* self, byte value);

AVMAPI AvmResult(char) AvmStreamReadChar(AvmStream* self);
AVMAPI AvmResult(void) AvmStreamWriteChar(AvmStream* self, char character);

AVMAPI AvmResult(AvmString) AvmStreamReadLine(AvmStream* self);
AVMAPI AvmResult(void) AvmStreamWriteLine(AvmStream* self, AvmString* string);

// Ensure type size constraints.
static_assert_s(sizeof(AvmStream) == AVM_STREAM_SIZE);

#endif  // AVIUM_IO_H
