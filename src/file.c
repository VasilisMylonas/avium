#include "avium/file.h"
#include "avium/resources.h"

#include <stdio.h>
#include <string.h>

#ifdef AVM_LINUX
#    include <unistd.h>
#endif

AvmStream* AvmFileOpen(str path, AvmFileAccess access) {
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

// AvmResult(void) AvmFileReadAll(str path, size_t length, byte bytes[]) {
//     if (path == NULL) {
//         AvmPanic(PathNullMsg);
//     }

//     AvmStream* stream = AvmFileOpen(path, FileAccessRead);
//     AvmStreamRead()
// }

AvmResult(void) AvmFileWriteAll(str path, size_t length, byte bytes[]) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    AvmStream* stream = AvmFileOpen(path, FileAccessWrite);
    AvmResult(void) result = AvmStreamWrite(stream, length, bytes);
    AvmObjectDestroy(stream);
    return result;
}
