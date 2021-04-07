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

static void AvmMemoryStreamFlush(AvmMemoryStream* self) { (void)self; }

static AvmResult(void)
    AvmMemoryStreamRead(AvmMemoryStream* self, size_t length, byte bytes[]) {
    for (size_t i = 0; i < length; i++) {
        bytes[i] = self->_list._items[self->_position];
        self->_position++;
    }

    return AvmSuccess(void)();
}

static AvmResult(void)
    AvmMemoryStreamWrite(AvmMemoryStream* self, size_t length, byte bytes[]) {
    for (size_t i = 0; i < length; i++) {
        AvmArrayListPush(byte)(&self->_list, bytes[i]);
        self->_position++;
    }

    return AvmSuccess(void)();
}

static void AvmMemoryStreamSeek(AvmMemoryStream* self, _long offset,
                                AvmSeekOrigin origin) {
    switch (origin) {
        case SeekOriginCurrent:
            self->_position += offset;
            break;
        case SeekOriginBegin:
            self->_position = offset;
            break;
        case SeekOriginEnd:
            self->_position =
                AvmArrayListGetLength(byte)(&self->_list) - offset;
            break;
        default:
            AvmPanic(InvalidOriginMsg);
    }
}

static size_t AvmMemoryStreamGetPosition(AvmMemoryStream* self) {
    return self->_position;
}

AVM_TYPE(AvmMemoryStream,
         {
             [FUNC_FLUSH] = (AvmFunction)AvmMemoryStreamFlush,
             [FUNC_READ] = (AvmFunction)AvmMemoryStreamRead,
             [FUNC_WRITE] = (AvmFunction)AvmMemoryStreamWrite,
             [FUNC_SEEK] = (AvmFunction)AvmMemoryStreamSeek,
             [FUNC_GET_POSITION] = (AvmFunction)AvmMemoryStreamGetPosition,
         });

AvmStream* AvmStreamFromMemory(size_t capacity) {
    AvmMemoryStream* stream = malloc(sizeof(AvmMemoryStream));
    stream->_list = AvmArrayListNew(byte)(capacity);
    stream->_type = AVM_GET_TYPE(AvmMemoryStream);
    stream->_position = 0;
    return (AvmStream*)stream;
}
