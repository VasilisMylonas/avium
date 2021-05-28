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

#include <setjmp.h>

AVM_CLASS(AvmThrowContext, object, {
    AvmThrowContext* _prev;
    jmp_buf _jumpBuffer;
    object _thrownObject;
});

AVMAPI never __AvmRuntimeThrow(object value);
AVMAPI void __AvmRuntimePushThrowContext(AvmThrowContext*);
AVMAPI AvmThrowContext* __AvmRuntimePopThrowContext(void);
AVMAPI AvmThrowContext* __AvmRuntimeGetThrowContext(void);

#define try                                                                    \
    AvmThrowContext AVM_UNIQUE(__avmThrownContext);                            \
    __AvmRuntimePushThrowContext(&AVM_UNIQUE(__avmThrownContext));             \
    for (uint __avmTLC = 0; __avmTLC < 2; __avmTLC++)                          \
        if (__avmTLC == 1)                                                     \
        {                                                                      \
            if (__AvmRuntimeGetThrowContext()->_thrownObject == NULL)          \
            {                                                                  \
                __AvmRuntimePopThrowContext();                                 \
                break;                                                         \
            }                                                                  \
            __AvmRuntimeThrow(__AvmRuntimePopThrowContext()->_thrownObject);   \
        }                                                                      \
        else if (setjmp(__AvmRuntimeGetThrowContext()->_jumpBuffer) == 0)

// clang-format off
#define catch(T, name)                                                               \
    else if (instanceof(T, __AvmRuntimeGetThrowContext()->_thrownObject) &&    \
                 __AvmRuntimeGetThrowContext() != NULL &&                      \
                 (__avmTLC = 2) ==                                             \
                     2) for (object name =                                     \
                                 __AvmRuntimePopThrowContext()->_thrownObject; \
                             name != NULL;                                     \
                             name = NULL)
// clang-format on

#define throw(value) __AvmRuntimeThrow(value)

#endif // AVIUM_ERROR_H
