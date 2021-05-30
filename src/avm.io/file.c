#include "avium/file.h"

#include "avium/private/constants.h"
#include "avium/string.h"
#include "avium/testing.h"

#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef AVM_WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

AvmStream* AvmFileOpen(str path, AvmFileAccess access, AvmError** error)
{
    pre
    {
        assert(path != NULL);
        assert(access % 2 == 0);
        assert(access <= FileAccessAppend);
    }

    str mode = NULL;

    // Choose mode.
    switch (access)
    {
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
    }

    FILE* file = fopen(path, mode);

    if (file == NULL)
    {
        // Set the error indicator if it is not null.
        if (error != NULL)
        {
            *error = AvmErrorFromOSCode(errno);
        }
        return NULL;
    }

    return AvmStreamFromHandle(file);
}

bool AvmFileExists(str path)
{
    pre
    {
        assert(path != NULL);
    }

#ifdef AVM_WIN32
    return _access(path, 0) == 0;
#else
    return access(path, F_OK) == 0;
#endif
}

AvmError* AvmFileDelete(str path)
{
    pre
    {
        assert(path != NULL);
    }

    int status = remove(path);

    if (status != 0)
    {
        return AvmErrorFromOSCode(status);
    }

    return NULL;
}

AvmError* AvmFileMove(str source, str destination)
{
    pre
    {
        assert(source != NULL);
        assert(destination != NULL);
    }

    int status = rename(source, destination);

    if (status != 0)
    {
        return AvmErrorFromOSCode(status);
    }

    return NULL;
}

AvmError* AvmFileCopy(str source, str destination)
{
    pre
    {
        assert(source != NULL);
        assert(destination != NULL);
    }

    (void)source;
    (void)destination;

    // TODO
    throw(AvmErrorNew(InternalError));
}

static AvmError* AvmFilePerform(str path,
                                size_t length,
                                byte buffer[],
                                AvmFileAccess access)
{
    pre
    {
        assert(path != NULL);
        assert(buffer != NULL);
    }

    if (length == 0)
    {
        return NULL;
    }

    AvmError* error = NULL;
    AvmStream* stream = AvmFileOpen(path, access, &error);

    if (error != NULL)
    {
        return error;
    }

    switch (access)
    {
    case FileAccessRead:
        error = AvmStreamRead(stream, length, buffer);
        break;
    case FileAccessWrite:
    case FileAccessAppend:
        error = AvmStreamWrite(stream, length, buffer);
        break;
    default:
        throw(AvmErrorNew(InternalError));
        break;
    }

    return error;
}

AvmError* AvmFileReadAll(str path, size_t length, byte buffer[])
{
    return AvmFilePerform(path, length, buffer, FileAccessRead);
}

AvmError* AvmFileWriteAll(str path, size_t length, byte buffer[])
{
    return AvmFilePerform(path, length, buffer, FileAccessWrite);
}

AvmError* AvmFileAppendAll(str path, size_t length, byte buffer[])
{
    return AvmFilePerform(path, length, buffer, FileAccessAppend);
}

AvmError* AvmFileReadAllText(str path, AvmString* string)
{
    pre
    {
        assert(path != NULL);
        assert(string != NULL);
    }

#ifdef AVM_WIN32
    struct _stat buffer;
    if (_stat(path, &buffer) != 0)
    {
        return AvmErrorFromOSCode(errno);
    }
#else
    struct stat buffer;
    if (stat(path, &buffer) != 0)
    {
        return AvmErrorFromOSCode(errno);
    }
#endif

    const size_t length = AvmStringGetLength(string);

    AvmStringEnsureCapacity(string, buffer.st_size);
    AvmStringUnsafeSetLength(string, length + buffer.st_size);

    return AvmFileReadAll(path,
                          AvmStringGetCapacity(string),
                          (byte*)(AvmStringGetBuffer(string) + length));
}

AvmError* AvmFileWriteAllText(str path, AvmString* string)
{
    pre
    {
        assert(path != NULL);
        assert(string != NULL);
    }

    return AvmFileWriteAll(
        path, AvmStringGetCapacity(string), (byte*)AvmStringGetBuffer(string));
}

AvmError* AvmFileAppendAllText(str path, AvmString* string)
{
    pre
    {
        assert(path != NULL);
        assert(string != NULL);
    }

    return AvmFileAppendAll(
        path, AvmStringGetCapacity(string), (byte*)AvmStringGetBuffer(string));
}
