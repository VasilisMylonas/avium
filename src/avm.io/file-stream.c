#include "avium/io.h"

#include "avium/private/resources.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

#include <stdio.h>

AVM_CLASS(AvmFileStream, object, { AvmFileHandle _handle; });

static_assert_s(sizeof(AvmFileStream) == AVM_FILE_STREAM_SIZE);

static AvmError* AvmFileStreamFlush(AvmFileStream* self)
{
    int status = fflush(self->_handle);

    if (status != 0)
    {
        return AvmErrorFromOSCode(status);
    }

    return NULL;
}

static AvmError* AvmFileStreamWrite(AvmFileStream* self,
                                    size_t length,
                                    byte bytes[])
{
    if (fwrite(bytes, 1, length, self->_handle) != length)
    {
        return AvmErrorFromOSCode(ferror(self->_handle));
    }

    return NULL;
}

static AvmError* AvmFileStreamRead(AvmFileStream* self,
                                   size_t length,
                                   byte bytes[])
{
    if (fread(bytes, 1, length, self->_handle) != length)
    {
        return AvmErrorFromOSCode(ferror(self->_handle));
    }

    return NULL;
}

static AvmError* AvmFileStreamSeek(AvmFileStream* self,
                                   _long offset,
                                   AvmSeekOrigin origin)
{
    int status = 0;

    switch (origin)
    {
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
        throw(AvmErrorNew(InvalidOriginMsg));
    }

    if (status != 0)
    {
        return AvmErrorFromOSCode(status);
    }

    return NULL;
}

static size_t AvmFileStreamGetPosition(AvmFileStream* self)
{
    return (ulong)ftell(self->_handle);
}

static void AvmFileStreamDestroy(AvmFileStream* self)
{
    fclose(self->_handle);
}

static size_t AvmFileStreamGetLength(AvmFileStream* self)
{
    (void)self;
    // TODO: What would be a good value for an unknown length?
    return 0;
}

AVM_TYPE(AvmFileStream,
         object,
         {
             [FnEntryFlush] = (AvmFunction)AvmFileStreamFlush,
             [FnEntryRead] = (AvmFunction)AvmFileStreamRead,
             [FnEntryWrite] = (AvmFunction)AvmFileStreamWrite,
             [FnEntrySeek] = (AvmFunction)AvmFileStreamSeek,
             [FnEntryGetPosition] = (AvmFunction)AvmFileStreamGetPosition,
             [FnEntryDtor] = (AvmFunction)AvmFileStreamDestroy,
             [FnEntryGetLength] = (AvmFunction)AvmFileStreamGetLength,
         });

AvmStream* AvmStreamFromHandle(AvmFileHandle handle)
{
    pre
    {
        assert(handle != NULL);
    }

    AvmFileStream* stream = AvmAlloc(sizeof(AvmFileStream));
    stream->_handle = handle;
    stream->_type = typeid(AvmFileStream);
    return (AvmStream*)stream;
}
