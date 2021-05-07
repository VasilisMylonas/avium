#include "avium/testing.h"

#include <string.h>

static void PrintLocation(str file, uint line)
{
    const size_t length = strlen(file) + 5;
    AvmErrorf("%s:%u\n", file, line);
    for (size_t i = 0; i < length; i++)
    {
        AvmErrorf("-");
    }
    AvmErrorf("\n");
}

#define ASSERT_INT_BODY(op)                                       \
    if (x op y)                                                   \
    {                                                             \
        return;                                                   \
    }                                                             \
    PrintLocation(file, line);                                    \
    AvmErrorf("Assertion failed: %s " #op " %s\n", xName, yName); \
    AvmErrorf("Got:\n    %s: %i\n    %s: %i\n\n", xName, x, yName, y)

#define ASSERT_UINT_BODY(op)                                      \
    if (x op y)                                                   \
    {                                                             \
        return;                                                   \
    }                                                             \
    PrintLocation(file, line);                                    \
    AvmErrorf("Assertion failed: %s " #op " %s\n", xName, yName); \
    AvmErrorf("Got:\n    %s: %u\n    %s: %u\n\n", xName, x, yName, y)

#define ASSERT_STR_BODY(op)                                           \
    if (strcmp(x, y) op 0)                                            \
    {                                                                 \
        return;                                                       \
    }                                                                 \
    PrintLocation(file, line);                                        \
    AvmErrorf("Assertion failed: [%s] " #op " [%s]\n", xName, yName); \
    AvmErrorf("Got:\n    %s: %s\n    %s: %s\n\n", xName, x, yName, y)

void __AvmAssertEqInt(_long x, _long y, str file, uint line, str xName,
                      str yName)
{
    ASSERT_INT_BODY(==);
}

void __AvmAssertEqUint(ulong x, ulong y, str file, uint line, str xName,
                       str yName)
{
    ASSERT_UINT_BODY(==);
}

void __AvmAssertEqStr(str x, str y, str file, uint line, str xName, str yName)
{
    ASSERT_STR_BODY(==);
}

void __AvmAssertNeInt(_long x, _long y, str file, uint line, str xName,
                      str yName)
{
    ASSERT_INT_BODY(!=);
}

void __AvmAssertNeUint(ulong x, ulong y, str file, uint line, str xName,
                       str yName)
{
    ASSERT_UINT_BODY(!=);
}

void __AvmAssertNeStr(str x, str y, str file, uint line, str xName, str yName)
{
    ASSERT_STR_BODY(!=);
}

void __AvmAssertGtInt(_long x, _long y, str file, uint line, str xName,
                      str yName)
{
    ASSERT_INT_BODY(>);
}

void __AvmAssertGtUint(ulong x, ulong y, str file, uint line, str xName,
                       str yName)
{
    ASSERT_UINT_BODY(>);
}

void __AvmAssertGtStr(str x, str y, str file, uint line, str xName, str yName)
{
    ASSERT_STR_BODY(>);
}
