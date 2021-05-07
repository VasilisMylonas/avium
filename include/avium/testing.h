/**
 * @file avium/testing.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Testing utilities.
 * @version 0.2
 * @date 2021-05-07
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

#include <assert.h>

#ifdef AVM_NO_PRECOND
#define pre if (false)
#else
#define pre if (true)
#endif

#if defined NDEBUG || defined AVM_NO_POSTCOND
#define post if (false)
#else
#define post if (true)
#endif

#define assert_eq(x, y) assert(x == y)
#define assert_ne(x, y) assert(x != y)
#define assert_gt(x, y) assert(x > y)
#define assert_lt(x, y) assert(x < y)
#define assert_ge(x, y) assert(x >= y)
#define assert_le(x, y) assert(x <= y)

#endif // AVIUM_TESTING_H
