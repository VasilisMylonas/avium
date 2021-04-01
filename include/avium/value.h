/**
 * @file avium/value.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Primitive variant type.
 * @version 0.1
 * @date 2021-04-01
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

#ifndef AVIUM_VALUE_H
#define AVIUM_VALUE_H

#include "avium/prologue.h"

/// Describes the type currently held by an AvmValue.
typedef enum {
    ValueKindBool = 0,
    ValueKindInt,
    ValueKindUInt,
    ValueKindFloat,
    ValueKindStr,
    ValueKindChar,
} AvmValueKind;

/// A type that can contain any primitive type.
AVM_CLASS(AvmValue, object, {
    union {
        bool _asBool;
        double _asFloat;
        ulong _asUInt;
        _long _asInt;
        str _asStr;
        char _asChar;
    };
    AvmValueKind _kind;
});

/**
 * @brief Creates an AvmValue containing a boolean.
 *
 * @param value The boolean.
 * @return The created instance.
 */
AVMAPI AvmValue AvmValueFromBool(bool value);

/**
 * @brief Creates an AvmValue containing a character.
 *
 * @param value The character.
 * @return The created instance.
 */
AVMAPI AvmValue AvmValueFromChar(char value);

/**
 * @brief Creates an AvmValue containing a float.
 *
 * @param value The float.
 * @return The created instance.
 */
AVMAPI AvmValue AvmValueFromFloat(double value);

/**
 * @brief Creates an AvmValue containing an unsigned integer.
 *
 * @param value The unsigned integer.
 * @return The created instance.
 */
AVMAPI AvmValue AvmValueFromUInt(ulong value);

/**
 * @brief Creates an AvmValue containing a signed integer.
 *
 * @param value The signed integer.
 * @return The created instance.
 */
AVMAPI AvmValue AvmValueFromInt(_long value);

/**
 * @brief Creates an AvmValue containing a primitive string.
 *
 * @param value The primitive string.
 * @return The created instance.
 */
AVMAPI AvmValue AvmValueFromStr(str value);

/**
 * @brief Casts an AvmValue to a boolean.
 *
 * @pre Parameter @p self must not be null.
 *
 * @param self The AvmValue.
 * @return The boolean.
 */
AVMAPI bool AvmValueAsBool(AvmValue* self);

/**
 * @brief Casts an AvmValue to a character.
 *
 * @pre Parameter @p self must not be null.
 *
 * @param self The AvmValue.
 * @return The character.
 */
AVMAPI char AvmValueAsChar(AvmValue* self);

/**
 * @brief Casts an AvmValue to a float.
 *
 * @pre Parameter @p self must not be null.
 *
 * @param self The AvmValue.
 * @return The float.
 */
AVMAPI double AvmValueAsFloat(AvmValue* self);

/**
 * @brief Casts an AvmValue to an unsigned integer.
 *
 * @pre Parameter @p self must not be null.
 *
 * @param self The AvmValue.
 * @return The unsigned integer.
 */
AVMAPI ulong AvmValueAsUInt(AvmValue* self);

/**
 * @brief Casts an AvmValue to a signed integer.
 *
 * @pre Parameter @p self must not be null.
 *
 * @param self The AvmValue.
 * @return The signed integer.
 */
AVMAPI _long AvmValueAsInt(AvmValue* self);

/**
 * @brief Casts an AvmValue to a str.
 *
 * @pre Parameter @p self must not be null.
 *
 * @param self The AvmValue.
 * @return The str.
 */
AVMAPI str AvmValueAsStr(AvmValue* self);

/**
 * @brief Returns the type of value contained in an AvmValue.
 *
 * @pre Parameter @p self must not be null.
 *
 * @param self The AvmValue.
 * @return The type of value contained.
 */
AVMAPI AvmValueKind AvmValueGetKind(AvmValue* self);

static_assert_s(sizeof(AvmValue) == AVM_VALUE_SIZE);

#endif  // AVIUM_VALUE_H
