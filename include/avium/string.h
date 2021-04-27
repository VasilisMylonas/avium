/**
 * @file avium/string.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Dynamic string implementation.
 * @version 0.2.5
 * @date 2021-04-24
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

#ifndef AVIUM_STRING_H
#define AVIUM_STRING_H

#include "avium/types.h"
#include "avium/error.h"

// Type definition in types.h

#define AvmStringPush(self, x) \
    _Generic((x),                        \
        str: AvmStringPushStr,           \
        char*: AvmStringPushStr,         \
        char: AvmStringPushChar,         \
        AvmString*: AvmStringPushString \
    )(self, x)

#define AvmStringStartsWith(self, x) \
    _Generic((x),                        \
        str: AvmStringStartsWithStr,           \
        char*: AvmStringStartsWithStr,         \
        char: AvmStringStartsWithChar,         \
        AvmString*: AvmStringStartsWithString  \
    )(self, x)

#define AvmStringEndsWith(self, x) \
    _Generic((x),                        \
        str: AvmStringEndsWithStr,           \
        char*: AvmStringEndsWithStr,         \
        char: AvmStringEndsWithChar,         \
        AvmString*: AvmStringEndsWithString  \
    )(self, x)

#define AvmStringContains(self, x) \
    _Generic((x),                        \
        str: AvmStringContainsStr,           \
        char*: AvmStringContainsStr,         \
        char: AvmStringContainsChar,         \
        AvmString*: AvmStringContainsString  \
    )(self, x)

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
 * @pre Parameter @p contents must be a '\0' terminated string.
 * @pre Parameter @p contents must be not NULL.
 *
 * @param contents The contents of the AvmString.
 * @return The created instance.
 */
AVMAPI AvmString AvmStringFrom(str contents);

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

#ifdef AVM_ENABLE_ALIASES
#    define AvmItoa  AvmStringFromInt
#    define AvmUtoa  AvmStringFromUint
#    define AvmFtoa  AvmStringFromFloat
#    define AvmFtoa2 AvmStringFromFloat2
#endif

AVMAPI AvmString AvmStringFromInt(_long value);
AVMAPI AvmString AvmStringFromUint(ulong value, AvmNumericBase numericBase);
AVMAPI AvmString AvmStringFromFloat(double value);
AVMAPI AvmString AvmStringFromFloat2(float value);

/**
 * @brief Creates an AvmString from a raw string provided with its length.
 *
 * If @p length is 0 then the behavior is the same as AvmStringNew(0).
 *
 * @pre Parameter @p contents must be not NULL.
 *
 * @param length The length of the raw string.
 * @param contents The contents of the AvmString.
 * @return The created instance.
 */
AVMAPI AvmString AvmStringFromChars(size_t length, str contents);

/**
 * @brief Creates an AvmString by repeating a raw string.
 *
 * If @p count is 0 then the behavior is the same as AvmStringNew(0).
 *
 * @pre Parameter @p contents must be not NULL.
 *
 * @param contents The raw string to repeat.
 * @param count How many times to repeat.
 * @return The created instance.
 */
AVMAPI AvmString AvmStringRepeat(str contents, size_t count);

/**
 * @brief Creates an AvmString by repeating a character array provided with its
 * length.
 *
 * If @p count or @p length is 0 then the behavior is the same as
 * AvmStringNew(0).
 *
 * @pre Parameter @p contents must be not NULL.
 *
 * @param length The length of the array.
 * @param contents The character array to repeat.
 * @param count How many times to repeat.
 * @return The created instance.
 */
AVMAPI AvmString AvmStringRepeatChars(size_t length, str contents,
                                      size_t count);

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
 * @brief Gives access to the internal buffer of an AvmString.
 *
 * The returned pointer may be invalidated if the AvmString instance decides
 * to reallocate. For safe usage, do not modify the AvmString while holding
 * this pointer.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @return A pointer to the internal buffer.
 */
AVMAPI char* AvmStringAsPtr(AvmString* self);

/**
 * @brief Determines whether an AvmString is empty.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmString instance.
 * @return true if the string is empty, otherwise false.
 */
AVMAPI bool AvmStringIsEmpty(AvmString* self);

/**
 * @brief Returns the character at the specified index in an AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @param index The index of the character to get.
 * @param error Any error that occurred.
 *
 * @return The character at the specified index.
 */
AVMAPI char AvmStringCharAt(AvmString* self, size_t index, AvmError** error);

/**
 * @brief Calls a function for each character in an AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 * @pre Parameter @p function must be not NULL.
 *
 * @param self The AvmString instance.
 * @param function The function to call.
 */
AVMAPI void AvmStringForEach(AvmString* self, void (*function)(char));

/**
 * @brief Calls a function for each character in an AvmString providing it
 * additionally with the character index.
 *
 * @pre Parameter @p self must be not NULL.
 * @pre Parameter @p function must be not NULL.
 *
 * @param self The AvmString instance.
 * @param function The function to call.
 */
AVMAPI void AvmStringForEachEx(AvmString* self, void (*function)(char, size_t));

/**
 * @brief Applies a function for each character in an AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 * @pre Parameter @p function must be not NULL.
 *
 * @param self The AvmString instance.
 * @param function The function to call.
 */
AVMAPI void AvmStringMap(AvmString* self, char (*function)(char));

/**
 * @brief Applies a function for each character in an AvmString providing it
 * additionally with the character index.
 *
 * @pre Parameter @p self must be not NULL.
 * @pre Parameter @p function must be not NULL.
 *
 * @param self The AvmString instance.
 * @param function The function to call.
 */
AVMAPI void AvmStringMapEx(AvmString* self, char (*function)(char, size_t));

/**
 * @brief Applies a function for each character in an AvmString. Used for
 * compatibility with functions taking an int instead of a char.
 *
 * @pre Parameter @p self must be not NULL.
 * @pre Parameter @p function must be not NULL.
 *
 * @param self The AvmString instance.
 * @param function The function to call.
 */
AVMAPI void AvmStringMapCompat(AvmString* self, int (*function)(int));

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
 * @brief Pushes an array of characters to the end an AvmString.
 *
 * If the AvmString's length is not sufficient to hold the str then the
 * AvmString allocates new memory. The allocated memory may be larger than the
 * length of the str, in order to reduce total reallocations.
 *
 * @pre Parameter @p self must be not NULL.
 * @pre Parameter @p contents must be not NULL.
 *
 * @param self The AvmString instance.
 * @param length The length of the array.
 * @param contents The array of characters.
 */
AVMAPI void AvmStringPushChars(AvmString* self, size_t length, str contents);

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
 * @return The index or AvmInvalid.
 */
AVMAPI size_t AvmStringIndexOf(AvmString* self, char character);

/**
 * @brief Returns the index of the last occurrence of a character in an
 * AvmString.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @param character The character to find.
 *
 * @return The index or AvmInvalid.
 */
AVMAPI size_t AvmStringLastIndexOf(AvmString* self, char character);

/**
 * @brief Replaces the first occurrence of a character in an AvmString with
 * another.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @param oldCharacter The character to be replaced.
 * @param newCharacter The character to replace with.
 *
 * @return The index of the replaced character or AvmInvalid.
 */
AVMAPI size_t AvmStringReplace(AvmString* self, char oldCharacter,
                               char newCharacter);

/**
 * @brief Replaces the first N occurrences of a character in an AvmString with
 * another.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @param count The count of characters to replace.
 * @param oldCharacter The character to be replaced.
 * @param newCharacter The character to replace with.
 *
 * @return The count of the total replaced characters.
 */
AVMAPI size_t AvmStringReplaceN(AvmString* self, size_t count,
                                char oldCharacter, char newCharacter);

/**
 * @brief Replaces the last N occurrences of a character in an AvmString with
 * another.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @param count The count of characters to replace.
 * @param oldCharacter The character to be replaced.
 * @param newCharacter The character to replace with.
 *
 * @return The count of the total replaced characters.
 */
AVMAPI size_t AvmStringReplaceLastN(AvmString* self, size_t count,
                                    char oldCharacter, char newCharacter);

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
AVMAPI size_t AvmStringReplaceAll(AvmString* self, char oldCharacter,
                                  char newCharacter);

/**
 * @brief Replaces the last occurrence of a character in an AvmString with
 * another.
 *
 * @pre Parameter @p self must be not NULL.
 *
 * @param self The AvmString instance.
 * @param oldCharacter The character to be replaced.
 * @param newCharacter The character to replace with.
 *
 * @return The index or AvmInvalid.
 */
AVMAPI size_t AvmStringReplaceLast(AvmString* self, char oldCharacter,
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
 * @return The index or AvmInvalid.
 */
AVMAPI size_t AvmStringFind(AvmString* self, str substring);

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
 * @return The index or AvmInvalid.
 */
AVMAPI size_t AvmStringFindLast(AvmString* self, str substring);

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
 * @brief Clears the contents of an AvmString by setting its length to 0).
 *
 * @pre Parameter @p self must be not null.
 * @param self The AvmString instance.
 */
AVMAPI void AvmStringClear(AvmString* self);

/**
 * @brief Erases the contents of an AvmString, by filling the string with
 * zeroes.
 *
 * @pre Parameter @p self must be not null.
 * @param self The AvmString instance.
 */
AVMAPI void AvmStringErase(AvmString* self);

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

/**
 * @brief Destructures an AvmString instance.
 *
 * NULL can be passed to the parameters, to ignore the output.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmString instance.
 * @param[out] capacity The capacity.
 * @param[out] length The length.
 * @param[out] buffer The internal buffer.
 */
AVMAPI void AvmStringUnsafeDestruct(AvmString* self, size_t* capacity,
                                    size_t* length, char** buffer);

/**
 * @brief Creates an AvmString from the provided parts.
 *
 * No validation is done on the parameters. You are responsible for correct
 * usage.
 *
 * @param capacity The capacity of the buffer.
 * @param length The current length of the buffer.
 * @param buffer The heap buffer.
 * @return The created instance.
 */
AVMAPI AvmString AvmStringUnsafeFromRaw(size_t capacity, size_t length,
                                        char* buffer);

/**
 * @brief Determines whether an AvmString contains a character.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmString instance.
 * @param character The character to find.
 * @return true if the character is found, otherwise false.
 */
AVMAPI bool AvmStringContainsChar(AvmString* self, char character);

/**
 * @brief Determines whether an AvmString contains a raw string.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p contents must be not null.
 *
 * @param self The AvmString instance.
 * @param contents The string to find.
 * @return true if the string is found, otherwise false.
 */
AVMAPI bool AvmStringContainsStr(AvmString* self, str contents);

/**
 * @brief Determines whether an AvmString starts with a character.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmString instance.
 * @param character The character to find.
 * @return true if the AvmString starts with the character, otherwise false.
 */
AVMAPI bool AvmStringStartsWithChar(AvmString* self, char character);

/**
 * @brief Determines whether an AvmString starts with a raw string provided with
 * its length.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p contents must be not null.
 *
 * @param self The AvmString instance.
 * @param length The length of the string.
 * @param contents The string to find.
 *
 * @return true if the AvmString starts with the string, otherwise false.
 */
AVMAPI bool AvmStringStartsWithChars(AvmString* self, size_t length,
                                     str contents);

/**
 * @brief Determines whether an AvmString starts with a raw string.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p contents must be not null.
 *
 * @param self The AvmString instance.
 * @param contents The string to find.
 * @return true if the AvmString starts with the string, otherwise false.
 */
AVMAPI bool AvmStringStartsWithStr(AvmString* self, str contents);

/**
 * @brief Determines whether an AvmString starts with an AvmString.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p contents must be not null.
 *
 * @param self The AvmString instance.
 * @param contents The string to find.
 * @return true if the AvmString starts with the string, otherwise false.
 */
AVMAPI bool AvmStringStartsWithString(AvmString* self, AvmString* contents);

/**
 * @brief Determines whether an AvmString ends with a character.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmString instance.
 * @param character The character to find.
 * @return true if the AvmString ends with the character, otherwise false.
 */
AVMAPI bool AvmStringEndsWithChar(AvmString* self, char character);

/**
 * @brief Determines whether an AvmString ends with a raw string.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p contents must be not null.
 *
 * @param self The AvmString instance.
 * @param contents The string to find.
 * @return true if the AvmString ends with the string, otherwise false.
 */
AVMAPI bool AvmStringEndsWithStr(AvmString* self, str contents);

/**
 * @brief Determines whether an AvmString ends with a raw string provided with
 * its length.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p contents must be not null.
 *
 * @param self The AvmString instance.
 * @param length The length of the string.
 * @param contents The string to find.
 *
 * @return true if the AvmString ends with the string, otherwise false.
 */
AVMAPI bool AvmStringEndsWithChars(AvmString* self, size_t length,
                                   str contents);

/**
 * @brief Determines whether an AvmString ends with an AvmString.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p contents must be not null.
 *
 * @param self The AvmString instance.
 * @param contents The string to find.
 * @return true if the AvmString ends with the string, otherwise false.
 */
AVMAPI bool AvmStringEndsWithString(AvmString* self, AvmString* contents);

#ifndef DOXYGEN
// TODO
AVMAPI bool AvmStringContainsChars(AvmString* self, size_t length,
                                   str contents);

// TODO
AVMAPI bool AvmStringContainsString(AvmString* self, AvmString* contents);
#endif  // DOXYGEN

/**
 * @brief Writes formatted output into an AvmString.
 *
 * @pre Parameter @p format must be not null.
 *
 * @param format The format string.
 * @param ... The values to insert into the format string.
 *
 * @return The formatted string.
 */
AVMAPI AvmString AvmStringFormat(str format, ...);

/**
 * @brief Writes formatted output into an AvmString using a va_list.
 *
 * @pre Parameter @p format must be not null.
 *
 * @param format The format string.
 * @param args The va_list with the values to insert into the format string.
 *
 * @return The formatted string.
 */
AVMAPI AvmString AvmStringFormatV(str format, va_list args);

/**
 * @brief Reads formatted output from an AvmString.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p format must be not null.
 *
 * @param self The AvmString instance.
 * @param format The format string.
 * @param ... The values to insert into the format string.
 */
AVMAPI void AvmStringParse(AvmString* self, str format, ...);

/**
 * @brief Reads formatted output from an AvmString.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p format must be not null.
 *
 * @param self The AvmString instance.
 * @param format The format string.
 * @param args The va_list with the values to insert into the format string.
 */
AVMAPI void AvmStringParseV(AvmString* self, str format, va_list args);

#endif  // AVIUM_STRING_H
