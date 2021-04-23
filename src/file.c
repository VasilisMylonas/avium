#include "avium/file.h"
#include "avium/resources.h"

#include <stdio.h>
#include <string.h>

#ifdef AVM_WIN32
#    include <windows.h>
#else
#    include <unistd.h>
#endif

AvmResult(AvmStreamPtr) AvmFileOpen(str path, AvmFileAccess access) {
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
        return AvmFailure(AvmStreamPtr)(AvmErrorGetLast());
    }

    return AvmSuccess(AvmStreamPtr)(AvmStreamFromHandle(file));
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

AvmResult(void) AvmFileDelete(str path) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    int status = remove(path);

    if (status != 0) {
        return AvmFailure(void)(AvmErrorFromOSCode(status));
    }

    return AvmSuccess(void)();
}

AvmResult(void) AvmFileMove(str source, str destination) {
    if (source == NULL) {
        AvmPanic(SourceNullMsg);
    }

    if (destination == NULL) {
        AvmPanic(DestinationNullMsg);
    }

    int status = rename(source, destination);

    if (status != 0) {
        return AvmFailure(void)(AvmErrorFromOSCode(status));
    }

    return AvmSuccess(void)();
}

AvmResult(void) AvmFileCopy(str source, str destination) {
    if (source == NULL) {
        AvmPanic(SourceNullMsg);
    }

    if (destination == NULL) {
        AvmPanic(DestinationNullMsg);
    }

    // TODO
    AvmPanic("Not implemented!");
}

static AvmResult(void) AvmFilePerform(str path, size_t length, byte buffer[],
                                      AvmFileAccess access) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    AvmResult(AvmStreamPtr) res = AvmFileOpen(path, access);

    if (AvmIsFailure(AvmStreamPtr)(&res)) {
        return AvmFailure(void)(res._error);
    }

    AvmStream* stream = AvmUnwrap(AvmStreamPtr)(&res);
    AvmResult(void) result;
    switch (access) {
        case FileAccessRead:
            result = AvmStreamRead(stream, length, buffer);
            break;
        case FileAccessWrite:
        case FileAccessAppend:
            result = AvmStreamWrite(stream, length, buffer);
            break;
        default:
            AvmPanic("Internal error.");
            break;
    }

    AvmObjectDestroy(stream);
    return result;
}

AvmResult(void) AvmFileReadAll(str path, size_t length, byte buffer[]) {
    return AvmFilePerform(path, length, buffer, FileAccessRead);
}

AvmResult(void) AvmFileWriteAll(str path, size_t length, byte buffer[]) {
    return AvmFilePerform(path, length, buffer, FileAccessWrite);
}

AvmResult(void) AvmFileAppendAll(str path, size_t length, byte buffer[]) {
    return AvmFilePerform(path, length, buffer, FileAccessAppend);
}

AvmFileMetadata AvmFileGetMetadata(str path) {}
