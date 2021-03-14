/*
 * File: avium/fmt.h
 * Formatting utilities.
 *
 * About: Author
 * Vasilis Mylonas <vasilismylonas@protonmail.com>
 *
 * Section: License
 *
 * Copyright (C) 2021 Vasilis Mylonas
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

#ifndef AVIUM_FMT_H
#define AVIUM_FMT_H

#include "avium/core.h"

#define AVM_FMT_INT_DECIMAL  'i'
#define AVM_FMT_INT_HEX      'x'
#define AVM_FMT_INT_BINARY   'b'
#define AVM_FMT_INT_OCTAL    'o'
#define AVM_FMT_INT_UNSIGNED 'u'
#define AVM_FMT_INT_SIZE     'z'
#define AVM_FMT_CHAR         'c'
#define AVM_FMT_UNICODE      'U'
#define AVM_FMT_FLOAT        'f'
#define AVM_FMT_FLOAT_EXP    'e'
#define AVM_FMT_FLOAT_AUTO   'g'
#define AVM_FMT_STRING       's'
#define AVM_FMT_BOOL         't'
#define AVM_FMT_POINTER      'p'
#define AVM_FMT_VALUE        'v'
#define AVM_FMT_TYPE         'T'
#define AVM_FMT_SIZE         'Z'

/*
 * Enum: NumericBase
 *
 * NB_BINARY  - Base 2 (00010000)
 * NB_OCTAL   - Base 8 (20)
 * NB_DECIMAL - Base 10 (16)
 * NB_HEX     - Base 16 (10)
 */
typedef enum {
    NB_BINARY = 2,
    NB_OCTAL = 8,
    NB_DECIMAL = 10,
    NB_HEX = 16,
} NumericBase;

/*
 * Function: AvmItoa
 * Converts a signed integer to a string.
 *
 * Parameters:
 * value - The integer.
 *
 * Returns:
 * AvmString - The string representation.
 */
AVMAPI AvmString AvmItoa(_long value);

/*
 * Function: AvmUtoa
 * Converts a unsigned integer to a string with a specified base.
 *
 * Parameters:
 * value - The unsigned integer.
 * base - The base of the number.
 *
 * Returns:
 * AvmString - The string representation.
 */
AVMAPI AvmString AvmUtoa(ulong value, NumericBase base);

/*
 * Function: AvmFtoa
 * Converts a double to a string.
 *
 * Parameters:
 * value - The double value.
 *
 * Returns:
 * AvmString - The string representation.
 */
AVMAPI AvmString AvmFtoa(double value);

/*
 * Function: AvmFtoa2
 * Converts a float to a string (custom implementation).
 *
 * Parameters:
 * value - The float value.
 *
 * Returns:
 * AvmString - The string representation.
 */
AVMAPI AvmString AvmFtoa2(float value);

/*
 * Function: AvmVScanf
 * Reads formatted output from stdin using a va_list.
 *
 * Parameters:
 * format - The format string.
 * args - The va_list containing pointers to variables to read to.
 */
AVMAPI void AvmVScanf(str format, va_list args);

/*
 * Function: AvmScanf
 * Reads formatted output from stdin.
 *
 * Parameters:
 * format - The format string.
 * ... - Pointers to variables to read to.
 *
 * Note:
 * This function is inline.
 */
inline void AvmScanf(str format, ...) {
    va_list args;
    va_start(args, format);
    AvmVScanf(format, args);
    va_end(args);
}

/*
 * Function: AvmSscanf
 * Reads formatted output from an AvmString using a va_list.
 *
 * Parameters:
 * string - The AvmString.
 * format - The format string.
 * args - The va_list containing pointers to variables to read to.
 */
AVMAPI void AvmVSscanf(AvmString string, str format, va_list args);

/*
 * Function: AvmSscanf
 * Reads formatted output from an AvmString.
 *
 * Parameters:
 * string - The AvmString.
 * format - The format string.
 * ... - Pointers to variables to read to.
 *
 * Note:
 * This function is inline.
 */
inline void AvmSscanf(AvmString string, str format, ...) {
    va_list args;
    va_start(args, format);
    AvmVSscanf(string, format, args);
    va_end(args);
}

/*
 * Function: AvmVFscanf
 * Reads formatted output from a stream using a va_list.
 *
 * Parameters:
 * handle - The stream handle.
 * format - The format string.
 * args - The va_list containing pointers to variables to read to.
 */
AVMAPI void AvmVFscanf(void* handle, str format, va_list args);

/*
 * Function: AvmFscanf
 * Reads formatted output from a stream.
 *
 * Parameters:
 * handle - The stream handle.
 * format - The format string.
 * ... - Pointers to variables to read to.
 *
 * Note:
 * This function is inline.
 */
inline void AvmFscanf(void* handle, str format, ...) {
    va_list args;
    va_start(args, format);
    AvmVFscanf(handle, format, args);
    va_end(args);
}

/*
 * Function: AvmVSprintf
 * Writes formatted output into a string using a va_list.
 *
 * Parameters:
 * format - The format string.
 * args - The va_list containing values to insert into the format string.
 *
 * Returns:
 * AvmString - The formatted string.
 */
AVMAPI AvmString AvmVSprintf(str format, va_list args);

/*
 * Function: AvmSprintf
 * Writes formatted output into a string.
 *
 * Parameters:
 * format - The format string.
 * ... - The values to insert into the format string.
 *
 * Returns:
 * AvmString - The formatted string.
 *
 * Note:
 * This function is inline.
 */
inline AvmString AvmSprintf(str format, ...) {
    va_list args;
    va_start(args, format);
    AvmString s = AvmVSprintf(format, args);
    va_end(args);
    return s;
}

/*
 * Function: AvmVFprintf
 * Writes formatted output to a stream using a va_list.
 *
 * Parameters:
 * handle - The stream handle.
 * format - The format string.
 * args - The va_list containing the values to write.
 */
AVMAPI void AvmVFprintf(void* handle, str format, va_list args);

/*
 * Function: AvmFprintf
 * Writes formatted output to a stream.
 *
 * Parameters:
 * handle - The stream handle.
 * format - The format string.
 * ... - The values to write.
 *
 * Note:
 * This function is inline.
 */
inline void AvmFprintf(void* handle, str format, ...) {
    va_list args;
    va_start(args, format);
    AvmVFprintf(handle, format, args);
    va_end(args);
}

/*
 * Function: AvmVPrintf
 * Writes formatted output to stdout using a va_list.
 *
 * Parameters:
 * format - The format string.
 * args - The va_list containing the values to write.
 */
AVMAPI void AvmVPrintf(str format, va_list args);

/*
 * Function: AvmPrintf
 * Writes formatted output to stdout.
 *
 * Parameters:
 * format - The format string.
 * ... - The values to write.
 *
 * Note:
 * This function is inline.
 */
inline void AvmPrintf(str format, ...) {
    va_list args;
    va_start(args, format);
    AvmVPrintf(format, args);
    va_end(args);
}

/*
 * Function: AvmVErrorf
 * Writes formatted output to stderr using a va_list.
 *
 * Parameters:
 * format - The format string.
 * args - The va_list containing the values to write.
 */
AVMAPI void AvmVErrorf(str format, va_list args);

/*
 * Function: AvmErrorf
 * Writes formatted output to stderr.
 *
 * Parameters:
 * format - The format string.
 * ... - The values to write.
 *
 * Note:
 * This function is inline.
 */
inline void AvmErrorf(str format, ...) {
    va_list args;
    va_start(args, format);
    AvmVErrorf(format, args);
    va_end(args);
}

#endif  // AVIUM_FMT_H
