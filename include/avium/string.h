/**
 * @file avium/string.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Dynamic string implementation.
 * @version 0.2
 * @date 2021-03-24
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

/**
 * @brief Creates an AvmString with a specified capacity.
 *
 * The capacity is the number of characters that the string will be able to hold
 * until it needs to allocate more memory. If 0 is used as the capacity, then no
 * memory is allocated until a character is pushed.
 *
 * @param capacity The capacity of the AvmString.
 * @return The created instance.
 */
AVMAPI AvmString AvmStringNew(size_t capacity);

/**
 * @brief Creates an AvmString from a raw string.
 *
 * The raw string is copied into a heap buffer. The capacity of the resulting
 * string may be larger, in order to reduce total reallocations.
 *
 * @pre Parameter @p contents must be not NULL.
 *
 * @param contents The contents of the AvmString.
 * @return The created instance.
 */
AVMAPI AvmString AvmStringFrom(str contents);

/**
 * @brief Gives access to the internal buffer of an AvmString.
 *
 * The returned pointer may be invalidated if the AvmString instance decides to
 * reallocate. For safe usage, do not modify the AvmString while holding this
 * pointer.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @return A pointer to the internal buffer.
 */
AVMAPI char* AvmStringAsPtr(AvmString* self);

/**
 * @brief Returns the length of an AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @return The AvmString length.
 */
AVMAPI size_t AvmStringGetLength(AvmString* self);

/**
 * @brief Gets the capacity of an AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @return The AvmString capacity.
 */
AVMAPI size_t AvmStringGetCapacity(AvmString* self);

/**
 * @brief Pushes a character to the end of an AvmString.
 *
 * If the AvmString's length is less than its capacity, then the character is
 * simply appended to the end, otherwise the AvmString allocates new memory. The
 * allocated memory may be larger than one character, in order to reduce total
 * reallocations.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @param character The character to push.
 */
AVMAPI void AvmStringPushChar(AvmString* self, char character);

/**
 * @brief Pushes a str to the end an AvmString.
 *
 * If the AvmString's length is not sufficient to hold the str then the
 * AvmString allocates new memory. The allocated memory may be larger than the
 * length of the str, in order to reduce total reallocations.
 *
 * @pre Parameter @p self must be not NULL.
 * @pre Parameter @p contents must be not NULL.
 *
 * @param self The AvmString instance.
 * @param contents The str to push.
 */
AVMAPI void AvmStringPushStr(AvmString* self, str contents);

/**
 * @brief Pushes an AvmString to the end an AvmString.
 *
 * If the AvmString's length is not sufficient to hold the new contents then
 * the AvmString allocates new memory. The allocated memory may be larger than
 * the length of the new contents, in order to reduce total reallocations.
 *
 * @pre Parameter @p self must be not NULL.
 * @pre Parameter @p other must be not NULL.
 *
 * @param self The AvmString instance.
 * @param other The AvmString to push.
 */
AVMAPI void AvmStringPushString(AvmString* self, AvmString* other);

/**
 * @brief Returns the index of the first occurrence of a character in an
 * AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @param character The character to find.
 *
 * @return The index of the first occurrence of the character or nothing.
 */
AVMAPI AvmOptional(size_t) AvmStringIndexOf(AvmString* self, char character);

/**
 * @brief Returns the index of the last occurrence of a character in an
 * AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @param character The character to find.
 *
 * @return The index of the last occurrence of the character or nothing.
 */
AVMAPI AvmOptional(size_t)
    AvmStringLastIndexOf(AvmString* self, char character);

/**
 * @brief Replaces all occurrences of a character in an AvmString with another.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @param oldCharacter The character to be replaced.
 * @param newCharacter The character to replace with.
 *
 * @return The count of the total replaced characters.
 */
AVMAPI size_t AvmStringReplace(AvmString* self, char oldCharacter,
                               char newCharacter);

/**
 * @brief Returns the index of the first occurrence of a substring in an
 * AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 * @pre Parameter @p substring must be not NULL.
 *
 * @param self The AvmString instance.
 * @param substring The substring to find.
 *
 * @return The index of the first occurrence of the substring or nothing.
 */
AVMAPI AvmOptional(size_t) AvmStringFind(AvmString* self, str substring);

/**
 * @brief Returns the index of the last occurrence of a substring in an
 * AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 * @pre Parameter @p substring must be not NULL.
 *
 * @param self The AvmString instance.
 * @param substring The substring to find.
 *
 * @return The index of the last occurrence of the substring or nothing.
 */
AVMAPI AvmOptional(size_t) AvmStringFindLast(AvmString* self, str substring);

/**
 * @brief Returns the character at the specified index in an AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @param index The index of the character to get.
 *
 * @return The character at the specified index, or an error.
 */
AVMAPI AvmResult(char) AvmStringCharAt(AvmString* self, size_t index);

/**
 * @brief Reverses an AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 * @param self The AvmString instance.
 */
AVMAPI void AvmStringReverse(AvmString* self);

/**
 * @brief Converts all characters in an AvmString to uppercase.
 *
 * @pre Parameter @p self must be not NULL.
 * @param self The AvmString instance.
 */
AVMAPI void AvmStringToUpper(AvmString* self);

/**
 * Converts all characters in an AvmString to lowercase.
 *
 * @pre Parameter @p self must be not NULL.
 * @param self The AvmString instance.
 */
AVMAPI void AvmStringToLower(AvmString* self);

/**
 * @brief Changed the length value of an AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @param length The new length value.
 *
 * @warning This function can lead to undefined behavior if not used correctly.
 */
AVMAPI void AvmStringUnsafeSetLength(AvmString* self, size_t length);

#endif  // AVIUM_STRING_H
