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
#define pre if (true)
#define post if (true)

#include <assert.h>

#ifdef NDEBUG
#define assert_eq(x, y) ((void)0)
#define assert_ne(x, y) ((void)0)
#define assert_gt(x, y) ((void)0)
#define assert_lt(x, y) ((void)0)
#else
#define assert_eq(x, y)                  \
    _Generic((x), byte                   \
             : __AvmAssertEqUint, ushort \
             : __AvmAssertEqUint, uint   \
             : __AvmAssertEqUint, ulong  \
             : __AvmAssertEqUint, char   \
             : __AvmAssertEqInt, short   \
             : __AvmAssertEqInt, int     \
             : __AvmAssertEqInt, _long   \
             : __AvmAssertEqInt, str     \
             : __AvmAssertEqStr, char *  \
             : __AvmAssertEqStr)(x, y, __FILE__, __LINE__, #x, #y)

#define assert_ne(x, y)                  \
    _Generic((x), byte                   \
             : __AvmAssertNeUint, ushort \
             : __AvmAssertNeUint, uint   \
             : __AvmAssertNeUint, ulong  \
             : __AvmAssertNeUint, char   \
             : __AvmAssertNeInt, short   \
             : __AvmAssertNeInt, int     \
             : __AvmAssertNeInt, _long   \
             : __AvmAssertNeInt, str     \
             : __AvmAssertNeStr, char *  \
             : __AvmAssertNeStr)(x, y, __FILE__, __LINE__, #x, #y)

#define assert_gt(x, y)                  \
    _Generic((x), byte                   \
             : __AvmAssertGtUint, ushort \
             : __AvmAssertGtUint, uint   \
             : __AvmAssertGtUint, ulong  \
             : __AvmAssertGtUint, char   \
             : __AvmAssertGtInt, short   \
             : __AvmAssertGtInt, int     \
             : __AvmAssertGtInt, _long   \
             : __AvmAssertGtInt, str     \
             : __AvmAssertGtStr, char *  \
             : __AvmAssertGtStr)(x, y, __FILE__, __LINE__, #x, #y)

#define assert_lt(x, y) assert_gt(y, x)
#endif // NDEBUG

#ifndef DOXYGEN
AVMAPI void __AvmAssertEqInt(_long, _long, str, uint, str, str);
AVMAPI void __AvmAssertNeInt(_long, _long, str, uint, str, str);
AVMAPI void __AvmAssertGtInt(_long, _long, str, uint, str, str);
AVMAPI void __AvmAssertEqUint(ulong, ulong, str, uint, str, str);
AVMAPI void __AvmAssertNeUint(ulong, ulong, str, uint, str, str);
AVMAPI void __AvmAssertGtUint(ulong, ulong, str, uint, str, str);
AVMAPI void __AvmAssertEqStr(str, str, str, uint, str, str);
AVMAPI void __AvmAssertNeStr(str, str, str, uint, str, str);
AVMAPI void __AvmAssertGtStr(str, str, str, uint, str, str);
#endif // DOXYGEN

#endif // AVIUM_TESTING_H
