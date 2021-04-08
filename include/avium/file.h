#ifndef AVIUM_FILE_H
#define AVIUM_FILE_H

#include "avium/runtime.h"
#include "avium/io.h"

typedef enum {
    FileAccessRead,
    FileAccessWrite,
    FileAccessAppend,
} AvmFileAccess;

typedef void* AvmFileMetadata;

AVMAPI AvmStream* AvmFileOpen(str path, AvmFileAccess access);
AVMAPI bool AvmFileExists(str path);

AVMAPI AvmResult(void) AvmFileDelete(str path);
AVMAPI AvmResult(void) AvmFileMove(str source, str destination);
AVMAPI AvmResult(void) AvmFileCopy(str source, str destination);

AVMAPI AvmResult(void) AvmFileReadAll(str path, size_t length, byte bytes[]);
AVMAPI AvmResult(void) AvmFileWriteAll(str path, size_t length, byte bytes[]);
AVMAPI AvmResult(void) AvmFileAppendAll(str path, size_t length, byte bytes[]);

AVMAPI AvmResult(void) AvmFileReadAllText(str path, AvmString* string);
AVMAPI AvmResult(void) AvmFileWriteAllText(str path, AvmString* string);
AVMAPI AvmResult(void) AvmFileAppendAllText(str path, AvmString* string);

AVMAPI AvmFileMetadata AvmFileGetMetadata(str path);

#endif  // AVIUM_FILE_H
