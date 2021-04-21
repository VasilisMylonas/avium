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

/// Asserts whether a condition is true.
#define Assert(x) AVM_ASSERT_(x)

/// Asserts whether an object is of specific type.
#define AssertIsType(T, x) AVM_ASSERT_TYPE_(T, x)

/// Asserts whether an object is not of specific type.
#define AssertIsNotType(T, x) (!AVM_ASSERT_TYPE_(T, x))

/// Asserts whether a condition is false.
#define AssertNot(x) AVM_ASSERT_(!x)

/// Asserts whether two primitives are equal.
#define AssertEqual(x, y) AVM_ASSERT_(x == y)

/// Asserts whether two primitives are not equal.
#define AssertNotEqual(x, y) AVM_ASSERT_(x != y)

/// Asserts whether two strings are equal.
#define AssertStrEqual(x, y) AVM_ASSERT_(strcmp(x, y) == 0)

/// Asserts whether two strings are not equal.
#define AssertStrNotEqual(x, y) AVM_ASSERT_(strcmp(x, y) != 0)

/// Asserts whether two memory blocks are equal.
#define AssertMemEqual(x, y, size) AVM_ASSERT_(memcmp(x, y, size) == 0)

/// Asserts whether two memory blocks are not equal.
#define AssertMemNotEqual(x, y, size) AVM_ASSERT_(memcmp(x, y, size) != 0)

/// Asserts whether a pointer is null.
#define AssertNull(x) AVM_ASSERT_(x == NULL)

/// Asserts whether a pointer is not null.
#define AssertNotNull(x) AVM_ASSERT_(x != NULL)

/// Asserts whether two pointers are equal.
#define AssertPtrEqual(x, y) AVM_ASSERT_(((uptr)x) == ((uptr)y))

/// Asserts whether two pointers are not equal.
#define AssertPtrNotEqual(x, y) AVM_ASSERT_(((uptr)x) != ((uptr)y))

/// Asserts whether a number is between the range of a min and
/// max..
#define AssertInRange(x, min, max) AVM_ASSERT_(min <= x && x <= max)

/// Asserts whether a number is not between the range of a min
/// and max.
#define AssertNotInRange(x, min, max) AVM_ASSERT_(min > x && x > max)

/**
 * @brief This attributes marks a function as a test.
 *
 * Tests have the following signature:
 *
 * @code
 * __test TestSomething(object state);
 * @endcode
 */
#define __test static void

#ifndef DOXYGEN
#    define AVM_ASSERT_(expression)                                         \
        expression ? ((void)0)                                              \
                   : AvmPanicEx("Assertion failed: " #expression, __func__, \
                                __FILE__, __LINE__);

#    define AVM_ASSERT_TYPE_(T, x)                                            \
        istype(T, x) ? ((void)0)                                              \
                     : AvmPanicEx("Assertion failed: istype(" #T ", " #x ")", \
                                  __func__, __FILE__, __LINE__);
#endif  // DOXYGEN

#endif  // AVIUM_TESTING_H
