#include "avium/collections/array-list.h"
#include "avium/collections/list.h"

#include "avium/core.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

void TestListPush()
{
    const uint expectedLength = 20;
    const uint expectedCapacity = 20;
    const AvmType* const expectedType = typeid(char);
    char item = 'H';

    AvmArrayList arrayList = AvmArrayListNew(expectedType, expectedCapacity);

    for (uint i = 0; i < 20; i++)
    {
        AvmListPush(&arrayList, &item);
    }

    AvmPrintf("%v\n", &arrayList);

    assert(AvmListGetLength(&arrayList) == expectedLength);
    assert(AvmListGetCapacity(&arrayList) == expectedCapacity);
    assert(AvmListGetItemType(&arrayList) == expectedType);
}

#include "gc.h"

void main()
{
    TestListPush();
    GC_gcollect();
}
