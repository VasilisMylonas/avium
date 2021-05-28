/**
 * @file avium/io.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief IO utilities.
 * @version 0.1
 * @date 2021-04-15
 *
 * @copyright Copyright (c) 2021 Vasilis Mylonas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef AVIUM_IO_H
#define AVIUM_IO_H

#include "avium/config.h"

#ifdef AVM_USE_IO

#include "avium/core.h"

/// Represents a C file handle.
typedef void* AvmFileHandle;

/// Defines the origin point for a seek operation.
typedef enum
{
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

/**
 * @brief Reads bytes from an AvmStream.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p buffer must be not null.
 *
 * @param self The AvmStream instance.
 * @param length The number of bytes to read.
 * @param buffer The buffer to read the bytes to.
 *
 * @return The result of the IO operation.
 */
AVMAPI AvmError* AvmStreamRead(AvmStream* self, size_t length, byte buffer[]);

/**
 * @brief Writes bytes to an AvmStream.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p buffer must be not null.
 *
 * @param self The AvmStream instance.
 * @param length The number of bytes to write.
 * @param buffer The buffer from which to write the bytes.
 *
 * @return The result of the IO operation.
 */
AVMAPI AvmError* AvmStreamWrite(AvmStream* self, size_t length, byte buffer[]);

/**
 * @brief Flushes a stream, writing buffered data to the underlying device.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmStream instance.
 * @return The result of the IO operation.
 */
AVMAPI AvmError* AvmStreamFlush(AvmStream* self);

/**
 * @brief Seeks to a position in a stream.
 *
 * If @p origin is SeekOriginBegin, then the position is set to @p offset.
 * If @p origin is SeekOriginCurrent, then the position is incremented, or
 * decremented according to @p offset. If @p origin is SeekOriginEnd, then the
 * position is incremented, or decremented according to @p offset.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmStream instance.
 * @param offset The offset.
 * @param origin The origin point for seek operation.
 *
 * @return The result of the IO operation.
 */
AVMAPI AvmError* AvmStreamSeek(AvmStream* self,
                               _long offset,
                               AvmSeekOrigin origin);

/**
 * @brief Returns the current position in an AvmStream.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmStream instance.
 * @return The current position of the stream.
 */
AVMAPI size_t AvmStreamGetPosition(AvmStream* self);

/**
 * @brief Returns the length of an AvmStream.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmStream instance.
 * @return The length of the stream.
 */
AVMAPI size_t AvmStreamGetLength(AvmStream* self);

/**
 * @brief Reads a single byte from an AvmStream.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmStream instance.
 * @param[out] error Any error that occurs.
 * @return The byte.
 */
AVMAPI byte AvmStreamReadByte(AvmStream* self, AvmError** error);

/**
 * @brief Writes a single byte to an AvmStream.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmStream instance.
 * @param value The byte to write.
 * @return The result of the IO operation.
 */
AVMAPI AvmError* AvmStreamWriteByte(AvmStream* self, byte value);

/**
 * @brief Reads a single character from an AvmStream.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmStream instance.
 * @param[out] error Any error that occurs.
 * @return The character.
 */
AVMAPI char AvmStreamReadChar(AvmStream* self, AvmError** error);

/**
 * @brief Writes a single character to an AvmStream.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmStream instance.
 * @param value The character to write.
 * @return The result of the IO operation.
 */
AVMAPI AvmError* AvmStreamWriteChar(AvmStream* self, char value);

/**
 * @brief Reads a line of text from an AvmStream.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmStream instance.
 * @param[out] error Any error that occurs.
 * @return The result of the IO operation.
 */
AVMAPI AvmString AvmStreamReadLine(AvmStream* self, AvmError** error);

/**
 * @brief Writes a line of text from an AvmStream.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p string must be not null.
 *
 * @param self The AvmStream instance.
 * @param string The AvmString to write.s
 * @return The result of the IO operation.
 */
AVMAPI AvmError* AvmStreamWriteLine(AvmStream* self, AvmString* string);

#endif // AVM_USE_IO

#endif // AVIUM_IO_H
