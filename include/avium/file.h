#ifndef AVIUM_FILE_H
#define AVIUM_FILE_H

#include "avium/config.h"

#ifdef AVM_USE_IO

#    include "avium/runtime.h"
#    include "avium/io.h"

typedef enum {
    FileAccessRead = 2,
    FileAccessWrite = 4,
    FileAccessAppend = 8,
    FileAccessReadWrite = FileAccessRead | FileAccessWrite,
    FileAccessReadAppend = FileAccessRead | FileAccessAppend,
} AvmFileAccess;

typedef void* AvmFileMetadata;

AVMAPI AvmStream* AvmFileOpen(str path, AvmFileAccess access);
AVMAPI bool AvmFileExists(str path);

AVMAPI AvmError* AvmFileDelete(str path);
AVMAPI AvmError* AvmFileMove(str source, str destination);
AVMAPI AvmError* AvmFileCopy(str source, str destination);

AVMAPI AvmError* AvmFileReadAll(str path, size_t length, byte bytes[]);
AVMAPI AvmError* AvmFileWriteAll(str path, size_t length, byte bytes[]);
AVMAPI AvmError* AvmFileAppendAll(str path, size_t length, byte bytes[]);

AVMAPI AvmError* AvmFileReadAllText(str path, AvmString* string);
AVMAPI AvmError* AvmFileWriteAllText(str path, AvmString* string);
AVMAPI AvmError* AvmFileAppendAllText(str path, AvmString* string);

AVMAPI AvmFileMetadata AvmFileGetMetadata(str path);

#endif  // AVM_USE_IO

#endif  // AVIUM_FILE_H
