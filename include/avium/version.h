/**
 * @file avium/version.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Version information and type.
 * @version 0.2
 * @date 2021-04-04
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

#ifndef AVIUM_VERSION_H
#define AVIUM_VERSION_H

#include "avium/types.h"

/// A type representing a software version in the format: N.N.N-c
AVM_CLASS(AvmVersion, object, {
    ushort Major;
    ushort Minor;
    ushort Patch;
    char Tag;
});

/**
 * @brief Creates a new AvmVersion instance.
 *
 * @param major The version major number (incremented at breaking changes).
 * @param minor The version minor number (incremented at non-breaking changes).
 * @param patch The version patch number (incremented at bug fixes).
 * @param tag The version tag (a, b, etc).
 *
 * @return The created instance.
 */
AVMAPI AvmVersion AvmVersionFrom(ushort major, ushort minor, ushort patch,
                                 char tag);

/**
 * @brief Return an AvmVersion indicating the current runtime version.
 *
 * @return The current runtime version.
 */
AVMAPI AvmVersion AvmRuntimeGetVersion();

static_assert_s(sizeof(AvmVersion) == AVM_VERSION_SIZE);

#endif  // AVIUM_VERSION_H
