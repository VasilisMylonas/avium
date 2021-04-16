#include "avium/io.h"
#include "avium/resources.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AVM_CLASS(AvmFileStream, AvmStream, { AvmFileHandle _handle; });

static_assert_s(sizeof(AvmFileStream) == AVM_FILE_STREAM_SIZE);

static AvmResult(void) AvmFileStreamFlush(AvmFileStream* self) {
    int status = fflush(self->_handle);

    if (status != 0) {
        return AvmFailure(void)(AvmErrorFromOSCode(status));
    }

    return AvmSuccess(void)();
}

static AvmResult(void)
    AvmFileStreamWrite(AvmFileStream* self, size_t length, byte bytes[]) {
    if (fwrite(bytes, 1, length, self->_handle) != length) {
        return AvmFailure(void)(AvmErrorFromOSCode(ferror(self->_handle)));
    }

    return AvmSuccess(void)();
}

static AvmResult(void)
    AvmFileStreamRead(AvmFileStream* self, size_t length, byte bytes[]) {
    if (fread(bytes, 1, length, self->_handle) != length) {
        return AvmFailure(void)(AvmErrorFromOSCode(ferror(self->_handle)));
    }

    return AvmSuccess(void)();
}

static AvmResult(void)
    AvmFileStreamSeek(AvmFileStream* self, _long offset, AvmSeekOrigin origin) {
    int status = 0;

    switch (origin) {
        case SeekOriginCurrent:
            status = fseek(self->_handle, (long)offset, SEEK_CUR);
            break;
        case SeekOriginBegin:
            status = fseek(self->_handle, (long)offset, SEEK_SET);
            break;
        case SeekOriginEnd:
            status = fseek(self->_handle, (long)offset, SEEK_END);
            break;
        default:
            AvmPanic(InvalidOriginMsg);
    }

    if (status != 0) {
        return AvmFailure(void)(AvmErrorFromOSCode(status));
    }

    return AvmSuccess(void)();
}

static size_t AvmFileStreamGetPosition(AvmFileStream* self) {
    return (ulong)ftell(self->_handle);
}

static void AvmFileStreamDestroy(AvmFileStream* self) { fclose(self->_handle); }

static size_t AvmFileStreamGetLength(AvmFileStream* self) {
    (void)self;
    // TODO: What would be a good value for an unknown length?
    return 0;
}

AVM_TYPE(AvmFileStream,
         {
             [FnEntryFlush] = (AvmFunction)AvmFileStreamFlush,
             [FnEntryRead] = (AvmFunction)AvmFileStreamRead,
             [FnEntryWrite] = (AvmFunction)AvmFileStreamWrite,
             [FnEntrySeek] = (AvmFunction)AvmFileStreamSeek,
             [FnEntryGetPosition] = (AvmFunction)AvmFileStreamGetPosition,
             [FnEntryDtor] = (AvmFunction)AvmFileStreamDestroy,
             [FnEntryGetLength] = (AvmFunction)AvmFileStreamGetLength,
         });

AvmStream* AvmStreamFromHandle(AvmFileHandle handle) {
    if (handle == NULL) {
        AvmPanic(HandleNullMsg);
    }

    AvmFileStream* stream = AvmAlloc(sizeof(AvmFileStream));
    stream->_handle = handle;
    stream->_type = AVM_GET_TYPE(AvmFileStream);
    return (AvmStream*)stream;
}
