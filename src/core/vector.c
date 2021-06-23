#include "avium/vector.h"
#include "avium/typeinfo.h"
#include <string.h>

AVM_CLASS_TYPE(AvmVector, object, AVM_VTABLE_DEFAULT);

AvmVector AvmVectorNew(const AvmClass* type, uint capacity)
{
    pre
    {
        assert(type != NULL);
    }

    uint size = AvmTypeGetSize((const AvmType*)type);

    return (AvmVector){
        .__type = typeid(AvmVector),
        ._private =
            {
                .capacity = capacity,
                .items = capacity == 0
                             ? NULL
                             : AvmAlloc(size * capacity, size >= sizeof(void*)),
                .length = 0,
                .type = type,
            },
    };
}

uint AvmVectorGetLength(const AvmVector* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_private.length;
}

uint AvmVectorGetCapacity(const AvmVector* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_private.capacity;
}

const AvmClass* AvmVectorGetType(const AvmVector* self)
{
    pre
    {
        assert(self != NULL);
    }

    const AvmClass* type = self->_private.type;

    post
    {
        assert(type != NULL);
    }

    return type;
}

void AvmVectorReserve(AvmVector* self, uint capacity)
{
    pre
    {
        assert(self != NULL);
    }

    if (capacity == 0)
    {
        return;
    }

    uint vecLength = AvmVectorGetLength(self);
    uint vecCapacity = AvmVectorGetCapacity(self);

    if (vecLength + capacity <= vecCapacity)
    {
        return;
    }

    while (vecLength + capacity > vecCapacity)
    {
        vecCapacity *= AVM_VECTOR_GROW_FACTOR;
    }

    uint size = AvmTypeGetSize((const AvmType*)AvmVectorGetType(self));
    void* memory = AvmAlloc(vecCapacity * size, size >= sizeof(void*));

    memcpy(memory, self->_private.items, size * vecLength);
    self->_private.capacity = vecCapacity;
    self->_private.items = memory;
}

void AvmVectorInsert(AvmVector* self, uint index, object item)
{
    pre
    {
        assert(self != NULL);
        assert(index < AvmVectorGetLength(self));
        assert(item != NULL);
    }

    AvmVectorReserve(self, 1);
    uint size = AvmTypeGetSize((const AvmType*)AvmVectorGetType(self));

    void* source = ((byte*)self->_private.items) + index * size;
    void* destination = ((byte*)self->_private.items) + (index + 1) * size;
    uint moveSize = (AvmVectorGetLength(self) - index) * size;

    memmove(destination, source, moveSize);

    memcpy(source, item, size);
    self->_private.length++;
}

void AvmVectorPush(AvmVector* self, object item)
{
    pre
    {
        assert(self != NULL);
        assert(item != NULL);
    }

    AvmVectorReserve(self, 1);
    uint size = AvmTypeGetSize((const AvmType*)AvmVectorGetType(self));
    void* place =
        ((byte*)self->_private.items) + AvmVectorGetLength(self) * size;
    memcpy(place, item, size);
    self->_private.length++;
}

void AvmVectorRemove(AvmVector* self, uint index)
{
    pre
    {
        assert(self != NULL);
        assert(index < AvmVectorGetLength(self));
    }

    if (index == AvmVectorGetLength(self) - 1)
    {
        self->_private.length--;
        return;
    }

    uint size = AvmTypeGetSize((const AvmType*)AvmVectorGetType(self));

    void* destination = ((byte*)self->_private.items) + index * size;
    void* source = ((byte*)self->_private.items) + (index + 1) * size;
    uint moveSize = (AvmVectorGetLength(self) - index) * size;

    memmove(destination, source, moveSize);
    self->_private.length--;
}

object AvmVectorPeek(const AvmVector* self)
{
    pre
    {
        assert(self != NULL);
    }

    uint length = AvmVectorGetLength(self);
    uint size = AvmTypeGetSize((const AvmType*)AvmVectorGetType(self));

    return ((byte*)self->_private.items) + (length - 1) * size;
}

object AvmVectorPop(AvmVector* self)
{
    pre
    {
        assert(self != NULL);
        assert(AvmVectorGetLength(self) != 0);
    }

    self->_private.length--;

    uint length = AvmVectorGetLength(self);
    uint size = AvmTypeGetSize((const AvmType*)AvmVectorGetType(self));

    void* source = ((byte*)self->_private.items) + length * size;
    void* destination = AvmAlloc(size, true);
    memcpy(destination, source, size);
    return destination;
}

object AvmVectorGet(const AvmVector* self, uint index)
{
    pre
    {
        assert(self != NULL);
        assert(index < AvmVectorGetLength(self));
    }

    uint size = AvmTypeGetSize((const AvmType*)AvmVectorGetType(self));
    return ((byte*)self->_private.items) + index * size;
}
