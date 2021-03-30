#ifndef AVIUM_TESTING_H
#define AVIUM_TESTING_H

#include "avium/prologue.h"
#include <string.h>

#define AVM_ASSERT_(expression)                                         \
    expression ? ((void)0)                                              \
               : AvmPanicEx("Assertion failed: " #expression, __func__, \
                            __FILE__, __LINE__);

#define Assert(x)                     AVM_ASSERT_(x)
#define AssertNot(x)                  AVM_ASSERT_(!x)
#define AssertEqual(x, y)             AVM_ASSERT_(x == y)
#define AssertNotEqual(x, y)          AVM_ASSERT_(x != y)
#define AssertStrEqual(x, y)          AVM_ASSERT_(strcmp(x, y) == 0)
#define AssertStrNotEqual(x, y)       AVM_ASSERT_(strcmp(x, y) != 0)
#define AssertMemEqual(x, y, size)    AVM_ASSERT_(memcmp(x, y, size) == 0)
#define AssertMemNotEqual(x, y, size) AVM_ASSERT_(memcmp(x, y, size) != 0)
#define AssertNull(x)                 AVM_ASSERT_(x == NULL)
#define AssertNotNull(x)              AVM_ASSERT_(x != NULL)
#define AssertPtrEqual(x, y)          AVM_ASSERT_(((uptr)x) == ((uptr)y))
#define AssertPtrNotEqual(x, y)       AVM_ASSERT_(((uptr)x) != ((uptr)y))
#define AssertInRange(x, min, max)    AVM_ASSERT_(min <= x && x <= max)
#define AssertNotInRange(x, min, max) AVM_ASSERT_(min > x && x > max)

#define __test static void

#endif  // AVIUM_TESTING_H
