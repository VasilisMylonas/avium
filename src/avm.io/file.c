#include "avium/file.h"
#include "avium/private/resources.h"

#include <stdio.h>
#include <string.h>

#ifdef AVM_WIN32
#    include <windows.h>
#else
#    include <unistd.h>
#endif

AvmStream* AvmFileOpen(str path, AvmFileAccess access, AvmError** error) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    str mode = NULL;

    switch (access) {
        case FileAccessRead:
            mode = "r";
            break;
        case FileAccessWrite:
            mode = "w";
            break;
        case FileAccessAppend:
            mode = "a";
            break;
        case FileAccessReadWrite:
            mode = "r+";
            break;
        case FileAccessReadAppend:
            mode = "a+";
            break;
        default:
            AvmPanic("Parameter `access` was invalid.");
    }

    // TODO: May be null.
    FILE* file = fopen(path, mode);
    if (file == NULL) {
        if (error != NULL) {
            *error = AvmErrorGetLast();
        }
        return NULL;
    }

    return AvmStreamFromHandle(file);
}

bool AvmFileExists(str path) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

#ifdef AVM_LINUX
    return access(path, F_OK) == 0;
#else
    // TODO
    AvmPanic("TODO");
#endif
}

AvmError* AvmFileDelete(str path) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    int status = remove(path);

    if (status != 0) {
        return AvmErrorFromOSCode(status);
    }

    return NULL;
}

AvmError* AvmFileMove(str source, str destination) {
    if (source == NULL) {
        AvmPanic(SourceNullMsg);
    }

    if (destination == NULL) {
        AvmPanic(DestinationNullMsg);
    }

    int status = rename(source, destination);

    if (status != 0) {
        return AvmErrorFromOSCode(status);
    }

    return NULL;
}

AvmError* AvmFileCopy(str source, str destination) {
    if (source == NULL) {
        AvmPanic(SourceNullMsg);
    }

    if (destination == NULL) {
        AvmPanic(DestinationNullMsg);
    }

    // TODO
    AvmPanic("Not implemented!");
}

static AvmError* AvmFilePerform(str path, size_t length, byte buffer[],
                                AvmFileAccess access) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    AvmError* error = NULL;
    AvmStream* stream = AvmFileOpen(path, access, &error);

    if (error != NULL) {
        return error;
    }

    switch (access) {
        case FileAccessRead:
            error = AvmStreamRead(stream, length, buffer);
            break;
        case FileAccessWrite:
        case FileAccessAppend:
            error = AvmStreamWrite(stream, length, buffer);
            break;
        default:
            AvmPanic("Internal error.");
            break;
    }

    AvmObjectDestroy(stream);
    return error;
}

AvmError* AvmFileReadAll(str path, size_t length, byte buffer[]) {
    return AvmFilePerform(path, length, buffer, FileAccessRead);
}

AvmError* AvmFileWriteAll(str path, size_t length, byte buffer[]) {
    return AvmFilePerform(path, length, buffer, FileAccessWrite);
}

AvmError* AvmFileAppendAll(str path, size_t length, byte buffer[]) {
    return AvmFilePerform(path, length, buffer, FileAccessAppend);
}

AvmFileMetadata AvmFileGetMetadata(str path) {}
