#include "avium/testing.h"

#include <string.h>

static void PrintLocation(str file, uint line) {
    AvmErrorf("(%s:%u) ", file, line);
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

#define ASSERT_STR_BODY(op)                                           \
    if (strcmp(x, y) op 0) {                                          \
        return;                                                       \
    }                                                                 \
    PrintLocation(file, line);                                        \
    AvmErrorf("Assertion failed: [%s] " #op " [%s]\n", xName, yName); \
    AvmErrorf("Got:\n    %s: %s\n    %s: %s\n\n", xName, x, yName, y)

void __AvmAssert(bool condition, str file, uint line, str expression) {
    if (condition) {
        return;
    }

    PrintLocation(file, line);
    AvmErrorf("Assertion failed: %s\n\n", expression);
}

void __AvmAssertEqInt(_long x, _long y, str file, uint line, str xName,
                      str yName) {
    ASSERT_INT_BODY(==);
}

void __AvmAssertNeInt(_long x, _long y, str file, uint line, str xName,
                      str yName) {
    ASSERT_INT_BODY(!=);
}

void __AvmAssertGtInt(_long x, _long y, str file, uint line, str xName,
                      str yName) {
    ASSERT_INT_BODY(>);
}

void __AvmAssertLtInt(_long x, _long y, str file, uint line, str xName,
                      str yName) {
    ASSERT_INT_BODY(<);
}

void __AvmAssertEqUint(ulong x, ulong y, str file, uint line, str xName,
                       str yName) {
    if (x == y) {
        return;
    }

    ASSERT_UINT_BODY(==);
}

void __AvmAssertNeUint(ulong x, ulong y, str file, uint line, str xName,
                       str yName) {
    ASSERT_UINT_BODY(!=);
}

void __AvmAssertGtUint(ulong x, ulong y, str file, uint line, str xName,
                       str yName) {
    ASSERT_UINT_BODY(>);
}

void __AvmAssertLtUint(ulong x, ulong y, str file, uint line, str xName,
                       str yName) {
    ASSERT_UINT_BODY(<);
}

void __AvmAssertEqStr(str x, str y, str file, uint line, str xName, str yName) {
    ASSERT_STR_BODY(==);
}

void __AvmAssertNeStr(str x, str y, str file, uint line, str xName, str yName) {
    ASSERT_STR_BODY(!=);
}

void __AvmAssertGtStr(str x, str y, str file, uint line, str xName, str yName) {
    ASSERT_STR_BODY(>);
}

void __AvmAssertLtStr(str x, str y, str file, uint line, str xName, str yName) {
    ASSERT_STR_BODY(<);
}

void __AvmAssertNull(void* pointer, str file, uint line, str name) {
    if (pointer == NULL) {
        return;
    }

    PrintLocation(file, line);
    AvmErrorf("Assertion failed: %s == NULL\n\n", name);
}

void __AvmAssertNotNull(void* pointer, str file, uint line, str name) {
    if (pointer != NULL) {
        return;
    }

    PrintLocation(file, line);
    AvmErrorf("Assertion failed: %s != NULL\n\n", name);
}

void __AvmAssertInRangeInt(_long value, _long _min, _long _max, str file,
                           uint line, str name) {
    if (_min <= value && value <= _max) {
        return;
    }

    PrintLocation(file, line);
    AvmErrorf("Assertion failed: %i <= %s <= %i\n\n", _min, name, _max);
}

void __AvmAssertInRangeUint(ulong value, ulong _min, ulong _max, str file,
                            uint line, str name) {
    if (_min <= value && value <= _max) {
        return;
    }

    PrintLocation(file, line);
    AvmErrorf("Assertion failed: %u <= %s <= %u\n\n", _min, name, _max);
}

void __AvmAssertMemEq(byte* x, byte* y, size_t length, str file, uint line,
                      str xName, str yName) {
    if (memcmp(x, y, length) == 0) {
        return;
    }

    PrintLocation(file, line);
    AvmErrorf("Assertion failed: [%s] == [%s]\n", xName, yName);
    AvmErrorf("Memory was:\n    ");
    for (size_t i = 0; i < length; i++) {
        if (x[i] < 10) {  // One digit
            AvmErrorf("%i    ", x[i]);
        } else if (x[i] < 100) {
            AvmErrorf("%i   ", x[i]);
        } else {
            AvmErrorf("%i  ", x[i]);
        }
    }
    AvmErrorf("\n    ");
    for (size_t i = 0; i < length; i++) {
        if (y[i] < 10) {
            AvmErrorf("%i    ", y[i]);
        } else if (y[i] < 100) {
            AvmErrorf("%i   ", y[i]);
        } else {
            AvmErrorf("%i  ", y[i]);
        }
    }
    AvmErrorf("\n\n");
}
