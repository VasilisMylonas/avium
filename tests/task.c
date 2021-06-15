#include "avium/task.h"

void Task2(object value)
{
    (void)value;

    AvmPrintf("4 ");

    AvmTaskSwitchBack();

    AvmPrintf("6 ");

    AvmTaskSwitchBack();
}

void Task1(object value)
{
    (void)value;

    AvmPrintf("2 ");

    AvmTaskSwitchBack();

    const AvmTask* t = AvmTaskNew(Task2, NULL);
    AvmTaskSwitchTo(t);

    AvmPrintf("5 ");

    AvmTaskSwitchTo(t);

    AvmTaskSwitchBack();
}

void Main()
{
    const AvmTask* t = AvmTaskNew(Task1, NULL);

    AvmPrintf("1 ");

    AvmTaskSwitchTo(t);

    AvmPrintf("3 ");

    AvmTaskSwitchTo(t);

    AvmPrintf("7\n");
}
