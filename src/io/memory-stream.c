#include "avium/io.h"
#include "avium/resources.h"
#include "avium/array-list.h"

#include <stdio.h>
#include <stdlib.h>

AVM_CLASS(AvmMemoryStream, AvmStream, {
    size_t _position;
    AvmArrayList(byte) _list;
});

static_assert_s(sizeof(AvmMemoryStream) == AVM_MEMORY_STREAM_SIZE);

static AvmError* AvmMemoryStreamFlush(AvmMemoryStream* self) {
    (void)self;
    return NULL;
}

static AvmError* AvmMemoryStreamRead(AvmMemoryStream* self, size_t length,
                                     byte bytes[]) {
    for (size_t i = 0; i < length; i++) {
        bytes[i] = self->_list._items[self->_position];
        self->_position++;
    }

    return NULL;
}

static AvmError* AvmMemoryStreamWrite(AvmMemoryStream* self, size_t length,
                                      byte bytes[]) {
    for (size_t i = 0; i < length; i++) {
        AvmArrayListPush(byte)(&self->_list, bytes[i]);
        self->_position++;
    }

    return NULL;
}

static AvmError* AvmMemoryStreamSeek(AvmMemoryStream* self, _long offset,
                                     AvmSeekOrigin origin) {
    switch (origin) {
        case SeekOriginCurrent:
            self->_position += offset;
            break;
        case SeekOriginBegin:
            if (offset < 0) {
                return AvmErrorOfKind(ErrorKindRange);
            }
            self->_position = offset;
            break;
        case SeekOriginEnd:
            if (offset > 0) {
                return AvmErrorOfKind(ErrorKindRange);
            }
            self->_position =
                AvmArrayListGetCapacity(byte)(&self->_list) + offset;
            break;
        default:
            AvmPanic(InvalidOriginMsg);
    }

    return NULL;
}

static size_t AvmMemoryStreamGetPosition(AvmMemoryStream* self) {
    return self->_position;
}

static void AvmMemoryStreamDestroy(AvmMemoryStream* self) {
    AvmObjectDestroy(&self->_list);
}

static size_t AvmMemoryStreamGetLength(AvmMemoryStream* self) {
    return AvmArrayListGetCapacity(byte)(&self->_list);
}

AVM_TYPE(AvmMemoryStream,
         {
             [FnEntryFlush] = (AvmFunction)AvmMemoryStreamFlush,
             [FnEntryRead] = (AvmFunction)AvmMemoryStreamRead,
             [FnEntryWrite] = (AvmFunction)AvmMemoryStreamWrite,
             [FnEntrySeek] = (AvmFunction)AvmMemoryStreamSeek,
             [FnEntryGetPosition] = (AvmFunction)AvmMemoryStreamGetPosition,
             [FnEntryDtor] = (AvmFunction)AvmMemoryStreamDestroy,
             [FnEntryGetLength] = (AvmFunction)AvmMemoryStreamGetLength,
         });

AvmStream* AvmStreamFromMemory(size_t capacity) {
    AvmMemoryStream* stream = AvmAlloc(sizeof(AvmMemoryStream));
    stream->_list = AvmArrayListNew(byte)(capacity);
    stream->_type = typeid(AvmMemoryStream);
    stream->_position = 0;
    return (AvmStream*)stream;
}
