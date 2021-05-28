/**
 * @file avium/result.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Result and error types.
 * @version 0.2.2
 * @date 2021-05-27
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

#include "avium/typeinfo.h"
#include "avium/types.h"

/// A type representing an error.
AVM_INTERFACE(AvmError);

/// Represents a location in source code.
AVM_CLASS(AvmLocation, object, {
    str File;
    uint Line;
    uint Column;
});

/// Expands to an AvmLocation instance for the current location.
#define here                                                                   \
    (AvmLocation)                                                              \
    {                                                                          \
        .Column = 0, .File = __FILE__, .Line = __LINE__,                       \
        ._type = typeid(AvmLocation),                                          \
    }

/**
 * @brief Creates an AvmError with a message.
 *
 * @pre Parameter @p message must be not null.
 *
 * @param message The error message.
 * @return The created instance.
 */
AVMAPI AvmError* AvmErrorNew(str message);

/**
 * @brief Creates an AvmError from an OS code.
 *
 * If the code is 0 then a 'successful' error is created.
 *
 * @param code The error code.
 * @return The created instance.
 */
AVMAPI AvmError* AvmErrorFromOSCode(int code);

/// The callback function used by AvmCatch, ideally it should never return.
typedef void (*AvmThrowCallback)(AvmLocation, object, AvmString);

/**
 * @brief Throws an object to be caught by a handler, or the runtime itself.
 *
 * @pre Parameter @p value must be not null.
 *
 * @param location The location from which the object is thrown, typically here.
 * @param value The object to be thrown.
 * @return This function never returns.
 */
AVMAPI never AvmThrow(AvmLocation location, object value);

/**
 * @brief Sets a handler to catch incoming thrown objects.
 *
 * If @p handler is NULL, the default handler is restored.
 *
 * @param type The type of objects to catch.
 * @param handler The handler.
 */
AVMAPI void AvmCatch(const AvmType* type, AvmThrowCallback handler);

#define AvmThrowError(message) AvmThrow(here, AvmErrorNew(message))

#endif // AVIUM_ERROR_H
