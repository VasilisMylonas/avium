/**
 * @file avium/testing.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Testing utilities.
 * @version 0.2
 * @date 2021-04-05
 *
 * @copyright Copyright (c) 2021 Vasilis Mylonas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef AVIUM_TESTING_H
#define AVIUM_TESTING_H

#include "avium/types.h"
#include <string.h>
#include <avium/runtime.h>

AVMAPI void AvmAssert(bool condition, str file, uint line, str expression);
AVMAPI void AvmAssertEqInt(_long x, _long y, str file, uint line, str xName,
                           str yName);
AVMAPI void AvmAssertNeInt(_long x, _long y, str file, uint line, str xName,
                           str yName);
AVMAPI void AvmAssertGtInt(ulong x, ulong y, str file, uint line, str xName,
                           str yName);
AVMAPI void AvmAssertEqUint(ulong x, ulong y, str file, uint line, str xName,
                            str yName);
AVMAPI void AvmAssertNeUint(ulong x, ulong y, str file, uint line, str xName,
                            str yName);
AVMAPI void AvmAssertGtUint(ulong x, ulong y, str file, uint line, str xName,
                            str yName);
AVMAPI void AvmAssertEqStr(str x, str y, str file, uint line, str xName,
                           str yName);
AVMAPI void AvmAssertNeStr(str x, str y, str file, uint line, str xName,
                           str yName);
AVMAPI void AvmAssertGtStr(str x, str y, str file, uint line, str xName,
                           str yName);

AVMAPI void AvmAssertNull(void* pointer, str file, uint line, str name);
AVMAPI void AvmAssertNotNull(void* pointer, str file, uint line, str name);

#define AssertTrue(condition) \
    AvmAssert(condition, __FILE__, __LINE__, #condition)
#define AssertFalse(condition) AssertTrue(!condition)

#define AssertEq(x, y) \
    _Generic((x), byte                 \
             : AvmAssertEqUint, ushort \
             : AvmAssertEqUint, uint   \
             : AvmAssertEqUint, ulong  \
             : AvmAssertEqUint, char   \
             : AvmAssertEqInt, short   \
             : AvmAssertEqInt, int     \
             : AvmAssertEqInt, _long   \
             : AvmAssertEqInt, str     \
             : AvmAssertEqStr, char*   \
             : AvmAssertEqStr)(x, y, __FILE__, __LINE__, #x, #y)

#define AssertNe(x, y) \
    _Generic((x), byte                 \
             : AvmAssertNeUint, ushort \
             : AvmAssertNeUint, uint   \
             : AvmAssertNeUint, ulong  \
             : AvmAssertNeUint, char   \
             : AvmAssertNeInt, short   \
             : AvmAssertNeInt, int     \
             : AvmAssertNeInt, _long   \
             : AvmAssertNeInt, str     \
             : AvmAssertNeStr, char*   \
             : AvmAssertNeStr)(x, y, __FILE__, __LINE__, #x, #y)

#define AssertGt(x, y) \
    _Generic((x), byte                 \
             : AvmAssertGtUint, ushort \
             : AvmAssertGtUint, uint   \
             : AvmAssertGtUint, ulong  \
             : AvmAssertGtUint, char   \
             : AvmAssertGtInt, short   \
             : AvmAssertGtInt, int     \
             : AvmAssertGtInt, _long   \
             : AvmAssertGtInt, str     \
             : AvmAssertGtStr, char*  \
             : AvmAssertGtStr)(x, y, __FILE__, __LINE__, #x, #y)

#define AssertNull(pointer) AvmAssertNull(pointer, __FILE__, __LINE__, #pointer)
#define AssertNotNull(pointer) \
    AvmAssertNotNull(pointer, __FILE__, __LINE__, #pointer)

// /// Asserts whether a number is between the range of a min and
// /// max..
// #define AssertInRange(x, min, max) AVM_ASSERT_(min <= x && x <=
// max)

// /// Asserts whether a number is not between the range of a min
// /// and max.
// #define AssertNotInRange(x, min, max) AVM_ASSERT_(min > x && x >
// max)

// /// Asserts whether two memory blocks are equal.
// #define AssertMemEqual(x, y, size) AVM_ASSERT_(memcmp(x, y,
// size) == 0)

// /// Asserts whether two memory blocks are not equal.
// #define AssertMemNotEqual(x, y, size) AVM_ASSERT_(memcmp(x, y,
// size) != 0)

#endif  // AVIUM_TESTING_H
