#ifndef AVIUM_FILE_H
#define AVIUM_FILE_H

#include "avium/runtime.h"
#include "avium/io.h"

typedef enum {
    FileAccessRead = 2,
    FileAccessWrite = 4,
    FileAccessAppend = 8,
    FileAccessReadWrite = FileAccessRead | FileAccessWrite,
    FileAccessReadAppend = FileAccessRead | FileAccessAppend,
} AvmFileAccess;

AVMAPI AvmStream* AvmFileOpen(str path, AvmFileAccess access, AvmError** error);
AVMAPI bool AvmFileExists(str path);

AVMAPI AvmError* AvmFileDelete(str path);
AVMAPI AvmError* AvmFileMove(str source, str destination);
AVMAPI AvmError* AvmFileCopy(str source, str destination);

AVMAPI AvmError* AvmFileReadAll(str path, size_t length, byte buffer[]);
AVMAPI AvmError* AvmFileWriteAll(str path, size_t length, byte buffer[]);
AVMAPI AvmError* AvmFileAppendAll(str path, size_t length, byte buffer[]);

AVMAPI AvmError* AvmFileReadAllText(str path, AvmString* string);
AVMAPI AvmError* AvmFileWriteAllText(str path, AvmString* string);
AVMAPI AvmError* AvmFileAppendAllText(str path, AvmString* string);

// TODO
typedef void* AvmFileMetadata;

// TODO
AVMAPI AvmFileMetadata AvmFileGetMetadata(str path);

#endif  // AVIUM_FILE_H
