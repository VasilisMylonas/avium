/**
 * @file avium/fmt.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Formatted IO utilities.
 * @version 0.2
 * @date 2021-04-06
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef AVIUM_FMT_H
#define AVIUM_FMT_H

#include "avium/types.h"

/// Represents the base in which a number is represented.
typedef enum {
    /// Base 2 (00010000).
    NumericBaseBinary = 2,

    /// Base 8 (20).
    NumericBaseOctal = 8,

    /// Base 10 (16).
    NumericBaseDecimal = 10,

    /// Base 16 (10).
    NumericBaseHex = 16,
} AvmNumericBase;

AVMAPI AvmString AvmStringFromInt(_long value);
AVMAPI AvmString AvmStringFromUint(ulong value, AvmNumericBase numericBase);
AVMAPI AvmString AvmStringFromFloat(double value);
AVMAPI AvmString AvmStringFromFloat2(float value);

AVMAPI void AvmVScanf(str format, va_list args);
AVMAPI void AvmVPrintf(str format, va_list args);
AVMAPI void AvmVErrorf(str format, va_list args);

AVMAPI void AvmScanf(str format, ...);
AVMAPI void AvmPrintf(str format, ...);
AVMAPI void AvmErrorf(str format, ...);

#endif  // AVIUM_FMT_H
