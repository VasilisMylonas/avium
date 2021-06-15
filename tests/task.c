#include "avium/task.h"

static_assert(sizeof(AvmTask) == 48, "");

void Task2(object value)
{
    (void)value;

    AvmPrintf("4 ");

    AvmTaskReturn("Hello");

    AvmPrintf("6 ");

    AvmTaskSwitchBack();
}

void Task1(object value)
{
    (void)value;

    AvmPrintf("2 ");

    AvmTaskSwitchBack();

    AvmTask* t = AvmTaskNew(Task2, NULL);
    assert(AvmTaskSwitchTo(t) != NULL);

    AvmPrintf("5 ");

    assert(AvmTaskSwitchTo(t) == NULL);

    AvmTaskReturn("Example");
}

void Main()
{
    AvmTask* t = AvmTaskNew(Task1, NULL);

    AvmPrintf("1 ");

    assert(AvmTaskSwitchTo(t) == NULL);

    AvmPrintf("3 ");

    assert(AvmTaskSwitchTo(t) != NULL);

    AvmPrintf("7\n");
}
