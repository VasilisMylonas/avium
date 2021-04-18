#include "avium/file.h"
#include "avium/resources.h"

#include <stdio.h>
#include <string.h>

#ifdef AVM_LINUX
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

AvmResult(void) AvmFileReadAll(str path, size_t length, byte buffer[]) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    AvmResult(AvmStreamPtr) res = AvmFileOpen(path, FileAccessRead);

    if (AvmIsFailure(AvmStreamPtr)(&res)) {
        return AvmFailure(void)(res._error);
    }

    AvmStream* stream = AvmUnwrap(AvmStreamPtr)(&res);
    AvmStreamRead(stream, length, buffer);
    AvmObjectDestroy(stream);

    return AvmSuccess(void)();
}

AvmResult(void) AvmFileWriteAll(str path, size_t length, byte buffer[]) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    AvmResult(AvmStreamPtr) res = AvmFileOpen(path, FileAccessWrite);

    if (AvmIsFailure(AvmStreamPtr)(&res)) {
        return AvmFailure(void)(res._error);
    }

    AvmStream* stream = AvmUnwrap(AvmStreamPtr)(&res);

    AvmResult(void) result = AvmStreamWrite(stream, length, buffer);
    AvmObjectDestroy(stream);
    return result;
}
