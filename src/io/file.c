#include "avium/file.h"
#include "avium/private/resources.h"

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

// AvmError* AvmFileReadAll(str path, size_t length, byte bytes[]) {
//     if (path == NULL) {
//         AvmPanic(PathNullMsg);
//     }

//     AvmStream* stream = AvmFileOpen(path, FileAccessRead);
//     AvmStreamRead()
// }

AvmError* AvmFileWriteAll(str path, size_t length, byte bytes[]) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    AvmStream* stream = AvmFileOpen(path, FileAccessWrite);
    AvmError* result = AvmStreamWrite(stream, length, bytes);
    AvmObjectDestroy(stream);
    return result;
}
