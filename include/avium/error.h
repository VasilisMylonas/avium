/**
 * @file avium/result.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Result and error types.
 * @version 0.2.2
 * @date 2021-05-9
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

#include "avium/types.h"

/// A type representing an error.
AVM_INTERFACE(AvmError);

/// Describes the type of the error that occurred.
typedef enum
{
    ErrorKindArg,       ///< Received an invalid argument.
    ErrorKindRange,     ///< The provided index was out of range.
    ErrorKindMem,       ///< There was not enough memory to handle the request.
    ErrorKindInvalidOp, ///< The call was invalid for the current state.
    ErrorKindIO,        ///< An IO error occurred.
    ErrorKindSys,       ///< An unknown system error occurred.
    ErrorKindNotFound,  ///< A required resource was unavailable.
    ErrorKindRead,      ///< Could not perform the read operation.
    ErrorKindWrite,     ///< Could not perform the write operation.
} AvmErrorKind;

/**
 * @brief Returns the last error that occurred.
 *
 * The error is created based on the errno value and may create a 'successful'
 * error.
 *
 * @return A type that implements AvmError.
 */
AVMAPI AvmError* AvmErrorGetLast(void);

/**
 * @brief Creates an AvmError from an os code.
 *
 * If the code is 0 then a 'successful' error is created.
 *
 * @param code The error code.
 * @return The created instance.
 */
AVMAPI AvmError* AvmErrorFromOSCode(int code);

/**
 * @brief Creates an AvmError of a specific kind.
 *
 * @param kind The error kind.
 * @return The created instance.
 */
AVMAPI AvmError* AvmErrorOfKind(AvmErrorKind kind);

/**
 * @brief Returns the AvmError responsible for this error.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmError instance.
 * @return The source of the error.
 */
AVMAPI weakptr(AvmError) AvmErrorGetSource(AvmError* self);

/**
 * @brief Gets a backtrace of the stack during the creation of an error.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmError instance.
 * @return The backtrace.
 */
AVMAPI AvmString AvmErrorGetBacktrace(AvmError* self);

/**
 * @brief Aborts execution, printing a message and location information.
 *
 * @param message The message to print.
 */
#define AvmPanic(message) AvmPanicEx(message, __func__, __FILE__, __LINE__)

// clang-format off
#define try(call)                                                              \
    AvmError* AVM_UNIQUE(__avmError) = call;                                   \
    if (AVM_UNIQUE(__avmError) != NULL)                                        \
    {                                                                          \
        return AVM_UNIQUE(__avmError);                                         \
    }

#define catch(var, call)                          \
    AvmError* var = call; \
    if (var != NULL)
// clang-format on

/**
 * @brief Aborts execution, printing a message and location information.
 *
 * @param message The message to be printed.
 * @param function The function name.
 * @param file The file name.
 * @param line The line number.
 *
 * @return This function never returns.
 */
AVMAPI never AvmPanicEx(str message, str function, str file, uint line);

#endif // AVIUM_ERROR_H
