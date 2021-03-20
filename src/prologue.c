#include <stdio.h>
#include <stdlib.h>

#include "avium/internal.h"

never AvmPanic(str message, str function, str file, uint line) {
    fprintf(stderr, "Panic in file %s:%u in function %s()\n%s\n", file, line,
            function, message);
    abort();
}

AvmResult AvmSuccess(object value);
AvmResult AvmFailure(AvmErrorKind kind, str error);
str AvmResultGetError(AvmResult self);
bool AvmResultIsFailure(AvmResult self);
object AvmResultUnwrap(AvmResult self);
AvmOptional AvmSome(object value);
AvmOptional AvmNone();
bool AvmOptionalHasValue(AvmOptional self);
