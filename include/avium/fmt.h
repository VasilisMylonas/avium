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

/**
 * @brief Converts a signed integer to a string.
 *
 * @param value The integer value.
 * @return AvmString The string representation.
 */
AVMAPI AvmString AvmItoa(_long value);

/**
 * @brief Converts an unsigned integer to a string with a specified base.
 *
 * @param value The unsigned integer value.
 * @param numericBase The base of the number.
 *
 * @return The string representation.
 */
AVMAPI AvmString AvmUtoa(ulong value, AvmNumericBase numericBase);

/**
 * @brief Converts a double to a string.
 *
 * @param value The double value.
 * @return The string representation.
 */
AVMAPI AvmString AvmFtoa(double value);

/**
 * @brief Converts a float to a string (custom implementation).
 *
 * @param value The float value.
 * @return The string representation.
 */
AVMAPI AvmString AvmFtoa2(float value);

/**
 * @brief Reads formatted output from stdin using a va_list.
 *
 * @pre Parameter @p format must be not null.
 * @pre Parameter @p args must be not null.
 *
 * @param format The format string.
 * @param args The va_list containing pointers to variables to read to.
 */
AVMAPI void AvmVScanf(str format, va_list args);

/**
 * @brief Reads formatted output from stdin.
 *
 * @pre Parameter @p format must be not null.
 *
 * @param format The format string.
 * @param ... Pointers to variables to read to.
 */
AVMAPI inline void AvmScanf(str format, ...) {
    va_list args;
    va_start(args, format);
    AvmVScanf(format, args);
    va_end(args);
}

/**
 * @brief Reads formatted output from an AvmString using a va_list.
 *
 * @pre Parameter @p string must be not null.
 * @pre Parameter @p format must be not null.
 * @pre Parameter @p args must be not null.
 *
 * @param string The AvmString.
 * @param format The format string.
 * @param args The va_list containing pointers to variables to read to.
 */
AVMAPI void AvmVSscanf(AvmString* string, str format, va_list args);

/**
 * @brief Reads formatted output from an AvmString.
 *
 * @pre Parameter @p string must be not null.
 * @pre Parameter @p format must be not null.
 *
 * @param string The AvmString.
 * @param format The format string.
 * @param ... Pointers to variables to read to.
 */
AVMAPI inline void AvmSscanf(AvmString* string, str format, ...) {
    va_list args;
    va_start(args, format);
    AvmVSscanf(string, format, args);
    va_end(args);
}

/**
 * @brief Reads formatted output from a stream using a va_list.
 *
 * @pre Parameter @p handle must be not null.
 * @pre Parameter @p format must be not null.
 * @pre Parameter @p args must be not null.
 *
 * @param handle The stream handle.
 * @param format The format string.
 * @param args The va_list containing pointers to variables to read to.
 */
AVMAPI void AvmVFscanf(void* handle, str format, va_list args);

/**
 * @brief Reads formatted output from a stream.
 *
 * @pre Parameter @p handle must be not null.
 * @pre Parameter @p format must be not null.
 *
 * @param handle The stream handle.
 * @param format The format string.
 * @param ... Pointers to variables to read to.
 */
AVMAPI inline void AvmFscanf(void* handle, str format, ...) {
    va_list args;
    va_start(args, format);
    AvmVFscanf(handle, format, args);
    va_end(args);
}

/**
 * @brief Writes formatted output to a stream using a va_list.
 *
 * @pre Parameter @p handle must be not null.
 * @pre Parameter @p format must be not null.
 * @pre Parameter @p args must be not null.
 *
 * @param handle The stream handle.
 * @param format The format string.
 * @param args The va_list containing the values to write.
 */
AVMAPI void AvmVFprintf(void* handle, str format, va_list args);

/**
 * @brief Writes formatted output to a stream.
 *
 * @pre Parameter @p handle must be not null.
 * @pre Parameter @p format must be not null.
 *
 * @param handle The stream handle.
 * @param format The format string.
 * @param ... The values to write.
 */
AVMAPI inline void AvmFprintf(void* handle, str format, ...) {
    va_list args;
    va_start(args, format);
    AvmVFprintf(handle, format, args);
    va_end(args);
}

/**
 * @brief Writes formatted output to stdout using a va_list.
 *
 * @pre Parameter @p format must be not null.
 * @pre Parameter @p args must be not null.
 *
 * @param format The format string.
 * @param args The va_list containing the values to write.
 */
AVMAPI void AvmVPrintf(str format, va_list args);

/**
 * @brief Writes formatted output to stdout.
 *
 * @pre Parameter @p format must be not null.
 *
 * @param format The format string.
 * @param ... The values to write.
 */
AVMAPI inline void AvmPrintf(str format, ...) {
    va_list args;
    va_start(args, format);
    AvmVPrintf(format, args);
    va_end(args);
}

/**
 * @brief Writes formatted output to stderr using a va_list.
 *
 * @pre Parameter @p format must be not null.
 * @pre Parameter @p args must be not null.
 *
 * @param format The format string.
 * @param args The va_list containing the values to write.
 */
AVMAPI void AvmVErrorf(str format, va_list args);

/**
 * Writes formatted output to stderr.
 *
 * @pre Parameter @p format must be not null.
 *
 * @param format The format string.
 * @param ... The values to write.
 */
AVMAPI inline void AvmErrorf(str format, ...) {
    va_list args;
    va_start(args, format);
    AvmVErrorf(format, args);
    va_end(args);
}

#endif  // AVIUM_FMT_H
