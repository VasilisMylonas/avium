/*
 * File: avium/string.h
 * Dynamic string implementation.
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

#ifndef AVIUM_STRING_H
#define AVIUM_STRING_H

#include "avium/core.h"

#define AVM_STRING_GROWTH_FACTOR 2

/*
 * Define: AVM_STRING_NPOS
 * An invalid index into an AvmString.
 */
#define AVM_STRING_NPOS ((size_t)-1)

/*
 * Macro: AvmString
 * Shorthand for AvmString_ctor
 */
#define AvmString(capacity) AvmString_ctor(capacity)

/*
 * Function: AvmString_ctor
 * Creates an AvmString with a specified capacity.
 *
 * Parameters:
 * capacity - The capacity for the AvmString.
 *
 * Returns:
 * AvmString - The created AvmString.
 */
AVMAPI AvmString AvmString_ctor(size_t capacity);

/*
 * Function: AvmStringFrom
 * Creates an AvmString from a character array.
 *
 * Parameters:
 * contents - The contents of the AvmString.
 *
 * Returns:
 * AvmString - The created AvmString.
 */
AVMAPI AvmString AvmStringFrom(const char8_t* contents);

/*
 * Function: AvmStringAsPtr
 * Gives access to the internal character array of an AvmString.
 *
 * Parameters:
 * self - The AvmString.
 *
 * Returns:
 * char8_t* - A pointer to the character array.
 */
AVMAPI char8_t* AvmStringAsPtr(AvmString self);

/*
 * Function: AvmStringGetLength
 * Gets the length of an AvmString.
 *
 * Parameters:
 * self - The AvmString.
 *
 * Returns:
 * size_t - The AvmString length.
 */
AVMAPI size_t AvmStringGetLength(AvmString self);

/*
 * Function: AvmStringGetCapacity
 * Gets the capacity of an AvmString.
 *
 * Parameters:
 * self - The AvmString.
 *
 * Returns:
 * size_t - The AvmString capacity.
 */
AVMAPI size_t AvmStringGetCapacity(AvmString self);

/*
 * Function: AvmStringAppendChar
 * Appends a character to an AvmString.
 *
 * Parameters:
 * self - The AvmString.
 * character - The character to append.
 *
 * Returns:
 * AvmString - The new AvmString with the appended character.
 */
AVMAPI AvmString AvmStringAppendChar(AvmString self, char8_t character);

/*
 * Function: AvmStringAppend
 * Appends a character array to an AvmString.
 *
 * Parameters:
 * self - The AvmString.
 * characters - The character array to append.
 *
 * Returns:
 * AvmString - The new AvmString with the appended characters.
 */
AVMAPI AvmString AvmStringAppend(AvmString self, const char8_t* characters);

/*
 * Function: AvmStringConcat
 * Concatenates two AvmString instances.
 *
 * Parameters:
 * self - The first AvmString.
 * other - The second AvmString.
 *
 * Returns:
 * AvmString - The resulting AvmString.
 */
AVMAPI AvmString AvmStringConcat(AvmString self, AvmString other);

/*
 * Function: AvmStringIndexOf
 * Gets the index of the first occurrence of a character in an AvmString.
 *
 * Parameters:
 * self - The AvmString.
 * character - The character to find.
 *
 * Returns:
 * size_t - The index of the first occurrence of the character.
 *
 * Failure:
 * If the character is not found in the string then AVM_STRING_NPOS is returned.
 */
AVMAPI size_t AvmStringIndexOf(AvmString self, char8_t character);

/*
 * Function: AvmStringLastIndexOf
 * Gets the index of the last occurrence of a characater in an AvmString.
 *
 * Parameters:
 * self - The AvmString.
 * character - The character to find.
 *
 * Returns:
 * size_t - The index of the last occurrence of the character.
 *
 * Failure:
 * If the character is not found in the string then AVM_STRING_NPOS is returned.
 */
AVMAPI size_t AvmStringLastIndexOf(AvmString self, char8_t character);

/*
 * Function: AvmStringReplace
 * Replaces all occurrences of a character in an AvmString with another.
 *
 * Parameters:
 * self - The AvmString.
 * oldCharacter - The character to be replace.
 * newCharacter - The character to replace with.
 *
 * Returns:
 * size_t - The count of the total replaced characters.
 */
AVMAPI size_t AvmStringReplace(AvmString self, char8_t oldCharacter,
                               char8_t newCharacter);

AVMAPI size_t AvmStringFind(AvmString self, const char8_t* characters);
AVMAPI size_t AvmStringFindLast(AvmString self, const char8_t* characters);

AVMAPI char8_t AvmStringCharAt(AvmString self, size_t index);

/*
 * Function: AvmStringToUpper
 * Converts all characters in an AvmString to uppercase.
 *
 * Parameters:
 * self - The AvmString.
 */
AVMAPI void AvmStringToUpper(AvmString self);

/*
 * Function: AvmStringToLower
 * Converts all characters in an AvmString to lowercase.
 *
 * Parameters:
 * self - The AvmString.
 */
AVMAPI void AvmStringToLower(AvmString self);

/*
 * Function: AvmStringUnsafeSetLength
 * Changed the length value of an AvmString.
 *
 * Parameters:
 * self - The AvmString.
 * length - The new length value.
 *
 * Warning:
 * This function can lead to undefined behavior if not used correctly.
 */
AVMAPI void AvmStringUnsafeSetLength(AvmString self, size_t length);

#endif  // AVIUM_STRING_H
