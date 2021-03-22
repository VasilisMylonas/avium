#ifndef AVIUM_TESTING_H
#define AVIUM_TESTING_H

#include "avium/prologue.h"
#include <string.h>

#define AssertImpl(expression)                                        \
    expression ? ((void)0)                                            \
               : AvmPanic("Assertion failed: " #expression, __func__, \
                          __FILE__, __LINE__);

#define Assert(x)                     AssertImpl(x)
#define AssertNot(x)                  AssertImpl(!x)
#define AssertEqual(x, y)             AssertImpl(x == y)
#define AssertNotEqual(x, y)          AssertImpl(x != y)
#define AssertStringEqual(x, y)       AssertImpl(strcmp(x, y) == 0)
#define AssertStringNotEqual(x, y)    AssertImpl(strcmp(x, y) != 0)
#define AssertMemEqual(x, y, size)    AssertImpl(memcmp(x, y, size) == 0)
#define AssertMemNotEqual(x, y, size) AssertImpl(memcmp(x, y, size) != 0)
#define AssertNull(x)                 AssertImpl(x == NULL)
#define AssertNotNull(x)              AssertImpl(x != NULL)
#define AssertPtrEqual(x, y)          AssertImpl(((uptr)x) == ((uptr)y))
#define AssertPtrNotEqual(x, y)       AssertImpl(((uptr)x) != ((uptr)y))
#define AssertInRange(x, min, max)    AssertImpl(min <= x && x <= max)
#define AssertNotInRange(x, min, max) AssertImpl(min > x && x > max)

#endif  // AVIUM_TESTING_H
