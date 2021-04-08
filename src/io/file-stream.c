#include "avium/io.h"
#include "avium/resources.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AVM_CLASS(AvmFileStream, AvmStream, { AvmFileHandle _handle; });

static_assert_s(sizeof(AvmFileStream) == AVM_FILE_STREAM_SIZE);

static void AvmFileStreamFlush(AvmFileStream* self) { fflush(self->_handle); }

static AvmResult(void)
    AvmFileStreamWrite(AvmFileStream* self, size_t length, byte bytes[]) {
    if (fwrite(bytes, 1, length, self->_handle) != length) {
        return AvmFailure(void)(ErrorKindSys, strerror(ferror(self->_handle)));
    }

    return AvmSuccess(void)();
}

static AvmResult(void)
    AvmFileStreamRead(AvmFileStream* self, size_t length, byte bytes[]) {
    if (fread(bytes, 1, length, self->_handle) != length) {
        return AvmFailure(void)(ErrorKindSys, strerror(ferror(self->_handle)));
    }

    return AvmSuccess(void)();
}

static void AvmFileStreamSeek(AvmFileStream* self, _long offset,
                              AvmSeekOrigin origin) {
    switch (origin) {
        case SeekOriginCurrent:
            fseek(self->_handle, (long)offset, SEEK_CUR);
            break;
        case SeekOriginBegin:
            fseek(self->_handle, (long)offset, SEEK_SET);
            break;
        case SeekOriginEnd:
            fseek(self->_handle, (long)offset, SEEK_END);
            break;
        default:
            AvmPanic(InvalidOriginMsg);
    }
}

static size_t AvmFileStreamGetPosition(AvmFileStream* self) {
    return (ulong)ftell(self->_handle);
}

AVM_TYPE(AvmFileStream,
         {
             [FUNC_FLUSH] = (AvmFunction)AvmFileStreamFlush,
             [FUNC_READ] = (AvmFunction)AvmFileStreamRead,
             [FUNC_WRITE] = (AvmFunction)AvmFileStreamWrite,
             [FUNC_SEEK] = (AvmFunction)AvmFileStreamSeek,
             [FUNC_GET_POSITION] = (AvmFunction)AvmFileStreamGetPosition,
         });

AvmStream* AvmStreamFromHandle(AvmFileHandle handle) {
    if (handle == NULL) {
        AvmPanic(HandleNullMsg);
    }

    AvmFileStream* stream = malloc(sizeof(AvmFileStream));
    stream->_handle = handle;
    stream->_type = AVM_GET_TYPE(AvmFileStream);
    return (AvmStream*)stream;
}