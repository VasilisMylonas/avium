/**
 * @file avium/result.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Result and error types.
 * @version 0.2
 * @date 2021-04-10
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

#ifndef AVIUM_ERROR_H
#define AVIUM_ERROR_H

#include "avium/core.h"

/// A type representing an error.
AVM_INTERFACE(AvmError);

/// Describes the type of the error that occurred.
typedef enum
{
    /// Received an invalid argument.
    ErrorKindArg,

    /// The provided index was out of range.
    ErrorKindRange,

    /// There was not enough memory to handle the request.
    ErrorKindMem,

    /// The call was invalid for the current state.
    ErrorKindInvalidOp,

    /// An IO error occurred.
    ErrorKindIO,

    /// An unknown system error occurred.
    ErrorKindSys,

    /// A required resource was unavailable.
    ErrorKindNotFound,

    /// Could not perform the read operation.
    ErrorKindRead,

    /// Could not perform the write operation.
    ErrorKindWrite,
} AvmErrorKind;

/**
 * @brief Returns the last error that occurred.
 *
 * The error is created based on the errno value and may create a 'successful'
 * error.
 *
 * @return A type that implements AvmError.
 */
AVMAPI AvmError *AvmErrorGetLast(void);

/**
 * @brief Creates an AvmError from an os code.
 *
 * If the code is 0 then a 'successful' error is created.
 *
 * @param code The error code.
 * @return The created instance.
 */
AVMAPI AvmError *AvmErrorFromOSCode(int code);

/**
 * @brief Creates an AvmError of a specific kind.
 *
 * @param kind The error kind.
 * @return The created instance.
 */
AVMAPI AvmError *AvmErrorOfKind(AvmErrorKind kind);

/**
 * @brief Returns the AvmError responsible for this error.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmError instance.
 * @return The source of the error.
 */
AVMAPI weakptr(AvmError) AvmErrorGetSource(AvmError *self);

/**
 * @brief Gets a backtrace of the stack during the creation of an error.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmError instance.
 * @return The backtrace.
 */
AVMAPI AvmString AvmErrorGetBacktrace(AvmError *self);

#endif // AVIUM_ERROR_H
