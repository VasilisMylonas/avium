#include "avium/io.h"
#include <stdio.h>
#include <stdlib.h>

static void AvmMemoryStreamFlush(AvmMemoryStream* self) { (void)self; }

static void AvmMemoryStreamRead(AvmMemoryStream* self, size_t length,
                                byte bytes[]) {
    for (size_t i = 0; i < length; i++) {
        bytes[i] = self->_list._items[self->_base._position];
        self->_base._position++;
    }
}

static void AvmMemoryStreamWrite(AvmMemoryStream* self, size_t length,
                                 byte bytes[]) {
    for (size_t i = 0; i < length; i++) {
        AvmArrayListPush(byte)(&self->_list, bytes[i]);
        self->_base._position++;
    }
}

static void AvmMemoryStreamSeek(AvmMemoryStream* self, _long offset,
                                AvmSeekOrigin origin) {
    switch (origin) {
        case SeekOriginCurrent:
            self->_base._position += offset;
            break;
        case SeekOriginBegin:
            self->_base._position = offset;
            break;
        case SeekOriginEnd:
            self->_base._position =
                AvmArrayListGetLength(byte)(&self->_list) - offset;
            break;
        default:
            // Error
            break;
    }
}

static size_t AvmMemoryStreamGetPosition(AvmMemoryStream* self) {
    return self->_base._position;
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
    stream->_base._position = 0;
    return (AvmStream*)stream;
}
