#include "avium/io.h"

#include "avium/collections/array-list.h"
#include "avium/collections/list.h"
#include "avium/private/constants.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

AVM_CLASS(AvmMemoryStream, object, {
    size_t _position;
    AvmArrayList _list;
});

static AvmError* AvmMemoryStreamFlush(AvmMemoryStream* self)
{
    (void)self;
    return NULL;
}

static AvmError* AvmMemoryStreamRead(AvmMemoryStream* self,
                                     size_t length,
                                     byte bytes[])
{
    for (size_t i = 0; i < length; i++)
    {
        bytes[i] = self->_list._items[self->_position];
        self->_position++;
    }

    return NULL;
}

static AvmError* AvmMemoryStreamWrite(AvmMemoryStream* self,
                                      size_t length,
                                      byte bytes[])
{
    for (size_t i = 0; i < length; i++)
    {
        AvmListPush(&self->_list, bytes + i);
        self->_position++;
    }

    return NULL;
}

static AvmError* AvmMemoryStreamSeek(AvmMemoryStream* self,
                                     _long offset,
                                     AvmSeekOrigin origin)
{
    pre
    {
        assert(self != NULL);
        assert(origin <= SeekOriginEnd);
    }

    switch (origin)
    {
    case SeekOriginCurrent:
        self->_position += offset;
        break;
    case SeekOriginBegin:
        if (offset < 0)
        {
            // TODO
            throw(AvmErrorNew(RangeError));
            // return AvmErrorOfKind(ErrorKindRange);
        }
        self->_position = offset;
        break;
    case SeekOriginEnd:
        if (offset > 0)
        {
            // TODO
            throw(AvmErrorNew(RangeError));
            // return AvmErrorOfKind(ErrorKindRange);
        }
        self->_position = AvmListGetCapacity(&self->_list) + offset;
        break;
    }

    return NULL;
}

static size_t AvmMemoryStreamGetPosition(AvmMemoryStream* self)
{
    return self->_position;
}

static size_t AvmMemoryStreamGetLength(AvmMemoryStream* self)
{
    return AvmListGetCapacity(&self->_list);
}

static void AvmMemoryStreamFinalize(AvmMemoryStream* self)
{
    pre
    {
        assert(self != NULL);
    }

    // Unlike AvmFileStream we have no need to dispose non-memory resources.
}

AVM_TYPE(AvmMemoryStream,
         object,
         {
             [FnEntryFlush] = (AvmFunction)AvmMemoryStreamFlush,
             [FnEntryRead] = (AvmFunction)AvmMemoryStreamRead,
             [FnEntryWrite] = (AvmFunction)AvmMemoryStreamWrite,
             [FnEntrySeek] = (AvmFunction)AvmMemoryStreamSeek,
             [FnEntryGetPosition] = (AvmFunction)AvmMemoryStreamGetPosition,
             [FnEntryGetLength] = (AvmFunction)AvmMemoryStreamGetLength,
             [FnEntryFinalize] = (AvmFunction)AvmMemoryStreamFinalize,
         });

AvmStream* AvmStreamFromMemory(size_t capacity)
{
    AvmMemoryStream* stream = AvmAlloc(sizeof(AvmMemoryStream));
    stream->_list = AvmArrayListNew(typeid(byte), capacity);
    stream->_type = typeid(AvmMemoryStream);
    stream->_position = 0;
    return (AvmStream*)stream;
}
