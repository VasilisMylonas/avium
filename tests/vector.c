#include "avium/vector.h"
#include "avium/core.h"
#include "avium/typeinfo.h"

void TestVectorNew()
{
    const uint capacity = 5;
    const AvmClass* type = typeid(int);

    AvmVector vector = AvmVectorNew(type, capacity);

    assert(AvmVectorGetLength(&vector) == 0);
    assert(AvmVectorGetCapacity(&vector) == capacity);
    assert(AvmVectorGetType(&vector) == type);
}

void TestVectorReserve()
{
    const uint capacity = 10;
    const AvmClass* type = typeid(int);

    AvmVector vector = AvmVectorNew(type, capacity);
    AvmVectorReserve(&vector, capacity);

    assert(AvmVectorGetLength(&vector) == 0);
    assert(AvmVectorGetCapacity(&vector) == capacity);
    assert(AvmVectorGetType(&vector) == type);
}

void TestVectorPush()
{
    const uint capacity = 10;
    const AvmClass* type = typeid(int);
    const uint count = 10;

    AvmVector vector = AvmVectorNew(type, capacity);
    for (uint i = 0; i < count; i++)
    {
        AvmVectorPush(&vector, &i);
    }

    assert(AvmVectorGetLength(&vector) == count);
    assert(AvmVectorGetCapacity(&vector) == capacity);

    AvmVectorPush(&vector, (int[]){0});

    assert(AvmVectorGetLength(&vector) == count + 1);
    assert(AvmVectorGetCapacity(&vector) == capacity * AVM_VECTOR_GROW_FACTOR);

    assert(*(int*)AvmVectorPeek(&vector) == 0);
}

void TestVectorRemove()
{
    const uint capacity = 10;
    const AvmClass* type = typeid(int);
    const uint count = 10;

    AvmVector vector = AvmVectorNew(type, capacity);
    for (uint i = 0; i < count; i++)
    {
        AvmVectorPush(&vector, &i);
    }

    assert(AvmVectorGetLength(&vector) == count);
    assert(AvmVectorGetCapacity(&vector) == 10);

    AvmVectorRemove(&vector, 3);

    assert(AvmVectorGetLength(&vector) == count - 1);
    assert(AvmVectorGetCapacity(&vector) == 10);
    assert(*(int*)AvmVectorPeek(&vector) == 9);

    AvmVectorPop(&vector);

    assert(AvmVectorGetLength(&vector) == count - 2);
    assert(AvmVectorGetCapacity(&vector) == 10);
    assert(*(int*)AvmVectorPeek(&vector) == 8);
}

void Main()
{
    TestVectorNew();
    TestVectorReserve();
    TestVectorPush();
    TestVectorRemove();
}
