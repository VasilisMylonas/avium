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

#include "avium/runtime.h"

// TODO: These should be toggable via some config mechanism.
#define pre  if (true)
#define post if (true)

#include <assert.h>

#define AssertTrue(condition) \
    __AvmAssert(condition, __FILE__, __LINE__, #condition)

#define AssertFalse(condition) AssertTrue(!condition)

#define AssertEq(x, y) \
    _Generic((x), byte                 \
             : __AvmAssertEqUint, ushort \
             : __AvmAssertEqUint, uint   \
             : __AvmAssertEqUint, ulong  \
             : __AvmAssertEqUint, char   \
             : __AvmAssertEqInt, short   \
             : __AvmAssertEqInt, int     \
             : __AvmAssertEqInt, _long   \
             : __AvmAssertEqInt, str     \
             : __AvmAssertEqStr, char*   \
             : __AvmAssertEqStr)(x, y, __FILE__, __LINE__, #x, #y)

#define AssertNe(x, y) \
    _Generic((x), byte                 \
             : __AvmAssertNeUint, ushort \
             : __AvmAssertNeUint, uint   \
             : __AvmAssertNeUint, ulong  \
             : __AvmAssertNeUint, char   \
             : __AvmAssertNeInt, short   \
             : __AvmAssertNeInt, int     \
             : __AvmAssertNeInt, _long   \
             : __AvmAssertNeInt, str     \
             : __AvmAssertNeStr, char*   \
             : __AvmAssertNeStr)(x, y, __FILE__, __LINE__, #x, #y)

#define AssertGt(x, y) \
    _Generic((x), byte                 \
             : __AvmAssertGtUint, ushort \
             : __AvmAssertGtUint, uint   \
             : __AvmAssertGtUint, ulong  \
             : __AvmAssertGtUint, char   \
             : __AvmAssertGtInt, short   \
             : __AvmAssertGtInt, int     \
             : __AvmAssertGtInt, _long   \
             : __AvmAssertGtInt, str     \
             : __AvmAssertGtStr, char*  \
             : __AvmAssertGtStr)(x, y, __FILE__, __LINE__, #x, #y)

#define AssertLt(x, y) \
    _Generic((x), byte                 \
             : __AvmAssertLtUint, ushort \
             : __AvmAssertLtUint, uint   \
             : __AvmAssertLtUint, ulong  \
             : __AvmAssertLtUint, char   \
             : __AvmAssertLtInt, short   \
             : __AvmAssertLtInt, int     \
             : __AvmAssertLtInt, _long   \
             : __AvmAssertLtInt, str     \
             : __AvmAssertLtStr, char*  \
             : __AvmAssertLtStr)(x, y, __FILE__, __LINE__, #x, #y)

#define AssertInRange(min, max, value)                                     \
    _Generic((x), byte                                                     \
             : __AvmAssertInRangeUint, ushort                              \
             : __AvmAssertInRangeUint, uint                                \
             : __AvmAssertInRangeUint, ulong                               \
             : __AvmAssertInRangeUint, char                                \
             : __AvmAssertInRangeInt, short                                \
             : __AvmAssertInRangeInt, int                                  \
             : __AvmAssertInRangeInt, _long                                \
             : __AvmAssertInRangeInt)(value, min, max, __FILE__, __LINE__, \
                                      #value)

#define AssertNull(pointer) \
    __AvmAssertNull(pointer, __FILE__, __LINE__, #pointer)

#define AssertNotNull(pointer) \
    __AvmAssertNotNull(pointer, __FILE__, __LINE__, #pointer)

#define AssertMemEq(x, y, length) \
    __AvmAssertMemEq((byte*)(x), (byte*)(y), length, __FILE__, __LINE__, #x, #y)

#define test int main()

#ifndef DOXYGEN
AVMAPI void __AvmAssert(bool, str, uint, str);
AVMAPI void __AvmAssertEqInt(_long, _long, str, uint, str, str);
AVMAPI void __AvmAssertNeInt(_long, _long, str, uint, str, str);
AVMAPI void __AvmAssertGtInt(_long, _long, str, uint, str, str);
AVMAPI void __AvmAssertLtInt(_long, _long, str, uint, str, str);
AVMAPI void __AvmAssertEqUint(ulong, ulong, str, uint, str, str);
AVMAPI void __AvmAssertNeUint(ulong, ulong, str, uint, str, str);
AVMAPI void __AvmAssertGtUint(ulong, ulong, str, uint, str, str);
AVMAPI void __AvmAssertLtUint(ulong, ulong, str, uint, str, str);
AVMAPI void __AvmAssertEqStr(str, str, str, uint, str, str);
AVMAPI void __AvmAssertNeStr(str, str, str, uint, str, str);
AVMAPI void __AvmAssertGtStr(str, str, str, uint, str, str);
AVMAPI void __AvmAssertLtStr(str, str, str, uint, str, str);
AVMAPI void __AvmAssertMemEq(byte*, byte*, size_t, str, uint, str, str);
AVMAPI void __AvmAssertInRangeInt(_long, _long, _long, str, uint, str);
AVMAPI void __AvmAssertInRangeUint(ulong, ulong, ulong, str, uint, str);
AVMAPI void __AvmAssertNull(void*, str, uint, str);
AVMAPI void __AvmAssertNotNull(void*, str, uint, str);
#endif  // DOXYGEN

#endif  // AVIUM_TESTING_H
