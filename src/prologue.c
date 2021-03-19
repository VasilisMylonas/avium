#include <stdio.h>
#include <stdlib.h>

#include "avium/internal.h"

never AvmPanic(str message, str function, str file, uint line) {
    fprintf(stderr, "Panic in file %s:%u in function %s()\n%s\n", file, line,
            function, message);
    abort();
}

AVMAPI AvmResult AvmSuccess(object value);
AVMAPI AvmResult AvmFailure(AvmErrorKind kind);
AVMAPI bool AvmIsFailure(AvmResult self);
AVMAPI object AvmUnwrap(AvmResult self);
AVMAPI AvmOptional AvmSome(object value);
AVMAPI AvmOptional AvmNone();
AVMAPI bool AvmHasValue(AvmOptional optional);
