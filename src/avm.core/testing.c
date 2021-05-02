#include "avium/testing.h"
#include "avium/runtime.h"

static void PrintLocation(str file, uint line) {
    AvmErrorf("%s:%u\n", file, line);
}

#define ASSERT_INT_BODY(op)                                       \
    if (x op y) {                                                 \
        return;                                                   \
    }                                                             \
    PrintLocation(file, line);                                    \
    AvmErrorf("Assertion failed: %s " #op " %s\n", xName, yName); \
    AvmErrorf("Got:\n    %s: %i\n    %s: %i\n\n", xName, x, yName, y)

#define ASSERT_UINT_BODY(op)                                      \
    if (x op y) {                                                 \
        return;                                                   \
    }                                                             \
    PrintLocation(file, line);                                    \
    AvmErrorf("Assertion failed: %s " #op " %s\n", xName, yName); \
    AvmErrorf("Got:\n    %s: %u\n    %s: %u\n\n", xName, x, yName, y)

#define ASSERT_STR_BODY(op)                                                  \
    if (strcmp(x, y) op 0) {                                                 \
        return;                                                              \
    }                                                                        \
    PrintLocation(file, line);                                               \
    AvmErrorf("Assertion failed: strcmp(%s, %s) " #op " 0\n", xName, yName); \
    AvmErrorf("Got:\n    %s: %u\n    %s: %u\n\n", xName, x, yName, y)

void AvmAssert(bool condition, str file, uint line, str expression) {
    if (condition) {
        return;
    }

    PrintLocation(file, line);
    AvmErrorf("Assertion failed: %s\n", expression);
    AvmErrorf("\n\n");
}

void AvmAssertEqInt(_long x, _long y, str file, uint line, str xName,
                    str yName) {
    ASSERT_INT_BODY(==);
}

void AvmAssertNeInt(_long x, _long y, str file, uint line, str xName,
                    str yName) {
    ASSERT_INT_BODY(!=);
}

void AvmAssertGtInt(ulong x, ulong y, str file, uint line, str xName,
                    str yName) {
    ASSERT_INT_BODY(>);
}

void AvmAssertEqUint(ulong x, ulong y, str file, uint line, str xName,
                     str yName) {
    if (x == y) {
        return;
    }

    ASSERT_UINT_BODY(==);
}

void AvmAssertNeUint(ulong x, ulong y, str file, uint line, str xName,
                     str yName) {
    ASSERT_UINT_BODY(!=);
}

void AvmAssertGtUint(ulong x, ulong y, str file, uint line, str xName,
                     str yName) {
    ASSERT_UINT_BODY(>);
}

void AvmAssertEqStr(str x, str y, str file, uint line, str xName, str yName) {
    ASSERT_STR_BODY(==);
}

void AvmAssertNeStr(str x, str y, str file, uint line, str xName, str yName) {
    ASSERT_STR_BODY(!=);
}

void AvmAssertGtStr(str x, str y, str file, uint line, str xName, str yName) {
    ASSERT_STR_BODY(>);
}

void AvmAssertNull(void* pointer, str file, uint line, str name) {
    if (pointer == NULL) {
        return;
    }

    PrintLocation(file, line);
    AvmErrorf("Assertion failed: %s == NULL\n", name);
    AvmErrorf("\n\n");
}

void AvmAssertNotNull(void* pointer, str file, uint line, str name) {
    if (pointer != NULL) {
        return;
    }

    PrintLocation(file, line);
    AvmErrorf("Assertion failed: %s != NULL\n", name);
    AvmErrorf("\n\n");
}
