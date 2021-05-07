#ifndef AVIUM_FILE_H
#define AVIUM_FILE_H

#include "avium/config.h"

#ifdef AVM_USE_IO

#    include "avium/runtime.h"
#    include "avium/io.h"

/// Determines the mode that a file will be opened with.
typedef enum {
    FileAccessRead = 2,
    FileAccessWrite = 4,
    FileAccessAppend = 8,
    FileAccessReadWrite = FileAccessRead | FileAccessWrite,
    FileAccessReadAppend = FileAccessRead | FileAccessAppend,
} AvmFileAccess;

AVMAPI AvmStream* AvmFileOpen(str path, AvmFileAccess access, AvmError** error);

/**
 * @brief Determines whether a file is accessible.
 *
 * @pre Parameter @p path must be not null.
 *
 * @param path The path to the file.
 * @return true if the file is accessible, otherwise false.
 */
AVMAPI bool AvmFileExists(str path);

/**
 * @brief Deletes a file.
 *
 * @pre Parameter @p path must be not null.
 *
 * @param path The path to the file to delete.
 * @return Any error that occurred.
 */
AVMAPI AvmError* AvmFileDelete(str path);

/**
 * @brief Moves file to a different location.
 *
 * @pre Parameter @p source must be not null.
 * @pre Parameter @p destination must be not null.
 *
 * @param source The path to the file to move.
 * @param destination The path to the destination.
 * @return Any error that occurred.
 */
AVMAPI AvmError* AvmFileMove(str source, str destination);

/**
 * @brief Copies a file to a different location.
 *
 * @pre Parameter @p source must be not null.
 * @pre Parameter @p destination must be not null.
 *
 * @param source The path to the file to copy.
 * @param destination The path to the destination.
 * @return Any error that occurred.
 */
AVMAPI AvmError* AvmFileCopy(str source, str destination);

/**
 * @brief Reads the contents of a file to a buffer.
 *
 * @pre Parameter @p path must be not null.
 * @pre Parameter @p buffer must be not null.
 *
 * @param path The path to the file.
 * @param length The number of bytes to read.
 * @param buffer The buffer.
 *
 * @return Any error that occurred.
 */
AVMAPI AvmError* AvmFileReadAll(str path, size_t length, byte buffer[]);

/**
 * @brief Reads the contents of a buffer to a file.
 *
 * @pre Parameter @p path must be not null.
 * @pre Parameter @p buffer must be not null.
 *
 * @param path The path to the file.
 * @param length The number of bytes to write.
 * @param buffer The buffer.
 *
 * @return Any error that occurred.
 */
AVMAPI AvmError* AvmFileWriteAll(str path, size_t length, byte buffer[]);

/**
 * @brief Appends the contents of a buffer to a file.
 *
 * @pre Parameter @p path must be not null.
 * @pre Parameter @p buffer must be not null.
 *
 * @param path The path to the file.
 * @param length The number of bytes to append.
 * @param buffer The buffer.
 *
 * @return Any error that occurred.
 */
AVMAPI AvmError* AvmFileAppendAll(str path, size_t length, byte buffer[]);

AVMAPI AvmError* AvmFileReadAllText(str path, AvmString* string);
AVMAPI AvmError* AvmFileWriteAllText(str path, AvmString* string);
AVMAPI AvmError* AvmFileAppendAllText(str path, AvmString* string);

#endif  // AVM_USE_IO

#endif  // AVIUM_FILE_H
