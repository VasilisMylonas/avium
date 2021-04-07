#include "avium/io.h"
#include <stdio.h>
#include <stdlib.h>

static void AvmFileStreamFlush(AvmFileStream* self) { fflush(self->_handle); }

static void AvmFileStreamWrite(AvmFileStream* self, size_t length,
                               byte bytes[]) {
    fwrite(bytes, 1, length, self->_handle);
}

static void AvmFileStreamRead(AvmFileStream* self, size_t length,
                              byte bytes[]) {
    fread(bytes, 1, length, self->_handle);
}

static void AvmFileStreamSeek(AvmFileStream* self, _long offset,
                              AvmSeekOrigin origin) {
    fseek(self->_handle, (long)offset, origin);
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
    AvmFileStream* stream = malloc(sizeof(AvmFileStream));
    stream->_handle = handle;
    stream->_type = AVM_GET_TYPE(AvmFileStream);
    return (AvmStream*)stream;
}
