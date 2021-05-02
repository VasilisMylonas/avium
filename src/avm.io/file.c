#include "avium/file.h"

#include "avium/private/resources.h"
#include "avium/string.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef AVM_WIN32
#    include <io.h>
#else
#    include <unistd.h>
#endif

AvmStream* AvmFileOpen(str path, AvmFileAccess access, AvmError** error) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    str mode = NULL;

    // Choose mode.
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
            AvmPanic(InvalidAccessMsg);
    }

    FILE* file = fopen(path, mode);

    if (file == NULL) {
        // Set the error indicator if it is not null.
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

#ifdef AVM_WIN32
    return _access(path, 0) == 0;
#else
    return access(path, F_OK) == 0;
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
    AvmPanic(NotImplementedMsg);
}

static AvmError* AvmFilePerform(str path, size_t length, byte buffer[],
                                AvmFileAccess access) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    if (buffer == NULL) {
        AvmPanic(BufferNullMsg);
    }

    if (length == 0) {
        return NULL;
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
            AvmPanic(InternalErrorMsg);
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

AvmError* AvmFileReadAllText(str path, AvmString* string) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    if (string == NULL) {
        AvmPanic(StringNullMsg);
    }

#ifdef AVM_WIN32
    struct _stat buffer;
    if (_stat(path, &buffer) != 0) {
        return AvmErrorGetLast();
    }
#else
    struct stat buffer;
    if (stat(path, &buffer) != 0) {
        return AvmErrorGetLast();
    }
#endif

    const size_t length = AvmStringGetLength(string);

    AvmStringEnsureCapacity(string, buffer.st_size);
    AvmStringUnsafeSetLength(string, length + buffer.st_size);

    return AvmFileReadAll(path, AvmStringGetCapacity(string),
                          (byte*)(AvmStringAsPtr(string) + length));
}

AvmError* AvmFileWriteAllText(str path, AvmString* string) {
    if (string == NULL) {
        AvmPanic(StringNullMsg);
    }

    return AvmFileWriteAll(path, AvmStringGetCapacity(string),
                           (byte*)AvmStringAsPtr(string));
}

AvmError* AvmFileAppendAllText(str path, AvmString* string) {
    if (string == NULL) {
        AvmPanic(StringNullMsg);
    }

    return AvmFileAppendAll(path, AvmStringGetCapacity(string),
                            (byte*)AvmStringAsPtr(string));
}
