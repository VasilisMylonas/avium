#include "avium/task.h"

#include "avium/typeinfo.h"

#include <stdlib.h>

#include <deps/task.h>

AVM_CLASS_TYPE(AvmTask, object, AVM_VTABLE_DEFAULT);

AvmTask AvmTaskNew(AvmTaskEntryPoint entry, object value)
{
    return (AvmTask){
        .__type = typeid(AvmTask),
        ._private =
            {
                // TODO
                .state = NULL,
            },
    };
}

never AvmTaskExit(AvmExitCode code)
{
}

void AvmTaskYield()
{
}
