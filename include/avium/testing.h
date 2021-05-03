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

#define ASSERT_PARAMS str file, uint line, str xName, str yName

AVMAPI void AvmAssert(bool condition, str file, uint line, str expression);
AVMAPI void AvmAssertEqInt(_long x, _long y, ASSERT_PARAMS);
AVMAPI void AvmAssertNeInt(_long x, _long y, ASSERT_PARAMS);
AVMAPI void AvmAssertGtInt(_long x, _long y, ASSERT_PARAMS);
AVMAPI void AvmAssertLtInt(_long x, _long y, ASSERT_PARAMS);
AVMAPI void AvmAssertEqUint(ulong x, ulong y, ASSERT_PARAMS);
AVMAPI void AvmAssertNeUint(ulong x, ulong y, ASSERT_PARAMS);
AVMAPI void AvmAssertGtUint(ulong x, ulong y, ASSERT_PARAMS);
AVMAPI void AvmAssertLtUint(ulong x, ulong y, ASSERT_PARAMS);
AVMAPI void AvmAssertEqStr(str x, str y, ASSERT_PARAMS);
AVMAPI void AvmAssertNeStr(str x, str y, ASSERT_PARAMS);
AVMAPI void AvmAssertGtStr(str x, str y, ASSERT_PARAMS);
AVMAPI void AvmAssertLtStr(str x, str y, ASSERT_PARAMS);

AVMAPI void AvmAssertMemEq(byte* x, byte* y, size_t length, ASSERT_PARAMS);

AVMAPI void AvmAssertInRangeInt(_long value, _long _min, _long _max, str file,
                                uint line, str name);
AVMAPI void AvmAssertInRangeUint(ulong value, ulong _min, ulong _max, str file,
                                 uint line, str name);

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

#define AssertLt(x, y) \
    _Generic((x), byte                 \
             : AvmAssertLtUint, ushort \
             : AvmAssertLtUint, uint   \
             : AvmAssertLtUint, ulong  \
             : AvmAssertLtUint, char   \
             : AvmAssertLtInt, short   \
             : AvmAssertLtInt, int     \
             : AvmAssertLtInt, _long   \
             : AvmAssertLtInt, str     \
             : AvmAssertLtStr, char*  \
             : AvmAssertLtStr)(x, y, __FILE__, __LINE__, #x, #y)

#define AssertInRange(min, max, value)                                   \
    _Generic((x), byte                                                   \
             : AvmAssertInRangeUint, ushort                              \
             : AvmAssertInRangeUint, uint                                \
             : AvmAssertInRangeUint, ulong                               \
             : AvmAssertInRangeUint, char                                \
             : AvmAssertInRangeInt, short                                \
             : AvmAssertInRangeInt, int                                  \
             : AvmAssertInRangeInt, _long                                \
             : AvmAssertInRangeInt)(value, min, max, __FILE__, __LINE__, \
                                    #value)

#define AssertNull(pointer) AvmAssertNull(pointer, __FILE__, __LINE__, #pointer)
#define AssertNotNull(pointer) \
    AvmAssertNotNull(pointer, __FILE__, __LINE__, #pointer)

#define AssertMemEq(x, y, length) \
    AvmAssertMemEq((byte*)(x), (byte*)(y), length, __FILE__, __LINE__, #x, #y)

#define test int main()

#endif  // AVIUM_TESTING_H
