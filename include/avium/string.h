/**
 * @file avium/string.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Dynamic string implementation.
 * @version 0.1
 * @date 2021-03-18
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

#ifndef AVIUM_STRING_H
#define AVIUM_STRING_H

#include "avium/prologue.h"
#include "avium/collection.h"

// Type definition in prologue.h

#define AVM_STRING_GROWTH_FACTOR 2
#define AVM_STRING_NPOS          ((size_t)-1)

/**
 * @brief Creates an AvmString with a specified capacity.
 *
 * @param capacity The capacity for the AvmString.
 * @return AvmString The created AvmString.
 */
AVMAPI AvmString AvmStringNew(size_t capacity);

/**
 * @brief Creates an AvmString from a character array.
 *
 * @param contents The contents of the AvmString.
 * @return AvmString The created AvmString.
 */
AVMAPI AvmString AvmStringFrom(str contents);

/**
 * @brief Gives access to the internal character array of an AvmString.
 *
 * @param self The AvmString.
 * @return char* A pointer to the character array.
 */
AVMAPI char* AvmStringAsPtr(AvmString* self);

/**
 * @brief Gets the length of an AvmString.
 *
 * @param self The AvmString.
 * @return size_t The AvmString length.
 */
AVMAPI size_t AvmStringGetLength(AvmString* self);

/**
 * @brief Gets the capacity of an AvmString.
 *
 * @param self The AvmString.
 * @return size_t The AvmString capacity.
 */
AVMAPI size_t AvmStringGetCapacity(AvmString* self);

/**
 * @brief Pushes a character to an AvmString.
 *
 * @param self The AvmString.
 * @param character The character to push.
 */
AVMAPI void AvmStringPushChar(AvmString* self, char character);

/**
 * @brief Pushes an str to an AvmString.
 *
 * @param self The AvmString.
 * @param contents The str array to push.
 */
AVMAPI void AvmStringPushStr(AvmString* self, str contents);

/**
 * @brief Pushes an AvmString to an AvmString.
 *
 * @param self The AvmString.
 * @param other The AvmString to push.
 *
 * @return AvmString The resulting AvmString.
 */
AVMAPI void AvmStringPushString(AvmString* self, AvmString* other);

/**
 * @brief Gets the index of the first occurrence of a character in an AvmString.
 *
 * @param self The AvmString.
 * @param character The character to find.
 *
 * @return size_t The index of the first occurrence of the character.
 * @return AVM_STRING_NPOS The character was not in the string.
 */
AVMAPI size_t AvmStringIndexOf(AvmString* self, char character);

/**
 * @brief Gets the index of the last occurrence of a character in an AvmString.
 *
 * @param self The AvmString.
 * @param character The character to find.
 *
 * @return size_t The index of the last occurrence of the character.
 * @return AVM_STRING_NPOS The character was not in the string.
 */
AVMAPI size_t AvmStringLastIndexOf(AvmString* self, char character);

/**
 * @brief Replaces all occurrences of a character in an AvmString with another.
 *
 * @param self The AvmString.
 * @param oldCharacter The character to be replace.
 * @param newCharacter The character to replace with.
 *
 * @return size_t The count of the total replaced characters.
 */
AVMAPI size_t AvmStringReplace(AvmString* self, char oldCharacter,
                               char newCharacter);

/**
 * @brief Finds the first occurrence of a substring in an AvmString.
 *
 * @param self The AvmString.
 * @param characters The substring to find.
 * @return size_t The index of the first occurrence of the substring.
 * @return AVM_STRING_NPOS The substring was not in the string.
 */
AVMAPI size_t AvmStringFind(AvmString* self, str characters);

/**
 * @brief Finds the last occurrence of a substring in an AvmString.
 *
 * @param self The AvmString.
 * @param characters The substring to find.
 * @return size_t The index of the last occurrence of the substring.
 * @return AVM_STRING_NPOS The substring was not in the string.
 */
AVMAPI size_t AvmStringFindLast(AvmString* self, str characters);

/**
 * @brief Returns a character in an AvmString at a specified index.
 *
 * @param self The AvmString.
 * @param index The index of the character to get.
 *
 * @return char The character.
 */
AVMAPI char AvmStringCharAt(AvmString* self, size_t index);

/**
 * @brief Reverse an AvmString.
 *
 * @param self The AvmString.
 */
AVMAPI void AvmStringReverse(AvmString* self);

/**
 * @brief Converts all characters in an AvmString to uppercase.
 *
 * @param self The AvmString.
 */
AVMAPI void AvmStringToUpper(AvmString* self);

/**
 * Converts all characters in an AvmString to lowercase.
 *
 * @param self The AvmString.
 */
AVMAPI void AvmStringToLower(AvmString* self);

/**
 * @brief Changed the length value of an AvmString.
 *
 * @param self The AvmString.
 * @param length The new length value.
 *
 * @warning This function can lead to undefined behavior if not used correctly.
 */
AVMAPI void AvmStringUnsafeSetLength(AvmString* self, size_t length);

#endif  // AVIUM_STRING_H
