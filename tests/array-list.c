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

    assert(AvmListGetLength(&arrayList) == expectedLength);
    assert(AvmListGetCapacity(&arrayList) == expectedCapacity);
    assert(AvmListGetItemType(&arrayList) == expectedType);

    for (uint i = 0; i < AvmListGetLength(&arrayList); i++)
    {
        assert_eq(*(char*)AvmListItemAt(&arrayList, i), item);
    }

    char item2 = 'A';
    AvmListInsert(&arrayList, 4, &item2);

    assert(AvmListGetLength(&arrayList) == expectedLength + 1);
    assert(AvmListGetCapacity(&arrayList) ==
           expectedCapacity * AVM_ARRAY_LIST_GROWTH_FACTOR);

    assert_eq(AvmListIndexOf(&arrayList, &item2), 4);
}

void main()
{
    TestListPush();
}
