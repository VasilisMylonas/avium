#include "avium/collections/array-list.h"
#include "avium/collections/list.h"

#include "avium/core.h"
#include "avium/string.h"
#include "avium/typeinfo.h"

void main()
{
    AvmString s = AvmStringFrom("Hello");

    AvmArrayList arrayList = AvmArrayListNew(typeid(AvmString), 20);
    AvmList* list = &arrayList;
    AvmListPush(list, &s);
    AvmListPush(list, &s);
    AvmListPush(list, &s);
    AvmListPush(list, &s);

    AvmPrintf("%v\n", list);
}
