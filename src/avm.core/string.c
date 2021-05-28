#include "avium/string.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avium/core.h"
#include "avium/error.h"
#include "avium/private/errors.h"
#include "avium/private/resources.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

#ifdef AVM_HAVE_UCHAR_H
#include <uchar.h>
#endif

static AvmString AvmStringToString(AvmString* self)
{
    pre
    {
        assert(self != NULL);
    }

    // TODO: self->_buffer may be null.
    return AvmStringFrom(self->_buffer);
}

static object AvmStringClone(AvmString* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmString s = AvmStringFrom(self->_buffer);
    AvmString* ret = AvmAlloc(sizeof(AvmString));
    AvmCopy(&s, sizeof(AvmString), (byte*)ret);
    return ret;
}

static void AvmStringDestroy(AvmString* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmDealloc(self->_buffer);
}

AVM_TYPE(AvmString,
         object,
         {
             [FnEntryDtor] = (AvmFunction)AvmStringDestroy,
             [FnEntryClone] = (AvmFunction)AvmStringClone,
             [FnEntryToString] = (AvmFunction)AvmStringToString,
             [FnEntryGetLength] = (AvmFunction)AvmStringGetLength,
             [FnEntryGetCapacity] = (AvmFunction)AvmStringGetCapacity,
         });

void AvmStringEnsureCapacity(AvmString* self, uint capacity)
{
    pre
    {
        assert(self != NULL);
        assert(capacity <= AVM_MAX_STRING_SIZE);
    }

    if (capacity == 0)
    {
        return;
    }

    const uint totalRequired = self->_length + capacity;
    const uint newCapacity = self->_capacity * AVM_STRING_GROWTH_FACTOR;

    if (totalRequired > self->_capacity)
    {
        // TODO: There may be more efficient ways of doing this
        self->_capacity = newCapacity;

        if (newCapacity < totalRequired)
        {
            self->_capacity += capacity;
        }

        self->_buffer = AvmRealloc(self->_buffer, self->_capacity);
    }

    post
    {
        assert(self->_capacity >= self->_length);
        assert(self->_capacity >= capacity);
    }
}

//
// Constructors.
//

AvmString AvmStringNew(uint capacity)
{
    pre
    {
        assert(capacity <= AVM_MAX_STRING_SIZE);
    }

    return (AvmString){
        ._type = typeid(AvmString),
        ._length = 0,
        ._capacity = capacity,
        // If capacity is 0 we should not allocate any memory.
        ._buffer = capacity == 0 ? NULL : AvmAlloc(capacity),
    };
}

AvmString AvmStringFrom(str contents)
{
    pre
    {
        assert(contents != NULL);
    }

    const uint length = strlen(contents);

    if (length == 0)
    {
        return AvmStringNew(0);
    }

    AvmString self = AvmStringFromChars(length, contents);

    post
    {
        assert(self._length == length);
        assert(self._capacity == length * AVM_STRING_GROWTH_FACTOR);
        assert(self._buffer != NULL);
    }

    return self;
}

AvmString AvmStringFromChars(uint length, str contents)
{
    pre
    {
        assert(contents != NULL);
    }

    if (length == 0)
    {
        return AvmStringNew(0);
    }

    // We allocate more memory upfront to reduce reallocations.
    AvmString self = AvmStringNew(length * AVM_STRING_GROWTH_FACTOR);
    AvmStringPushChars(&self, length, contents);

    post
    {
        assert(self._length == length);
        assert(self._capacity == length * AVM_STRING_GROWTH_FACTOR);
        assert(self._buffer != NULL);
    }

    return self;
}

AvmString AvmStringFromInt(_long value)
{
    if (value == INTMAX_MIN)
    {
        return AvmStringFrom(LongMinRepr);
    }

    const bool isNegative = value < 0;
    if (isNegative)
    {
        value = -value;
    }

    AvmString s = AvmStringNew(8);

    uint i = 0;
    for (; value != 0; i++, value /= 10)
    {
        _long r = value % 10;
        AvmStringPushChar(&s, '0' + r);
    }

    if (i == 0)
    {
        AvmStringPushChar(&s, '0');
        i++;
    }

    if (isNegative)
    {
        AvmStringPushChar(&s, '-');
        i++;
    }

    AvmStringReverse(&s);

    post
    {
        assert(s._buffer != NULL);
        assert(s._length != 0);
        assert(s._capacity != 0);
    }

    return s;
}

AvmString AvmStringFromUint(ulong value, AvmNumericBase numericBase)
{
    switch (numericBase)
    {
    case NumericBaseBinary:
    case NumericBaseOctal:
    case NumericBaseDecimal:
    case NumericBaseHex:
        break;
    default:
        AvmThrowError(NumericBaseOutOfRangeMsg);
    }

    AvmString s = AvmStringNew(8);

    uint i = 0;
    for (; value != 0; i++, value /= numericBase)
    {
        _long r = value % numericBase;

        if (r >= 10)
        {
            AvmStringPushChar(&s, 'A' + (r - 10));
        }
        else
        {
            AvmStringPushChar(&s, '0' + r);
        }
    }

    if (i == 0)
    {
        AvmStringPushChar(&s, '0');
        i++;
    }

    AvmStringReverse(&s);

    post
    {
        assert(s._buffer != NULL);
        assert(s._length != 0);
        assert(s._capacity != 0);
    }

    return s;
}

typedef union {
    float value;

    struct
    {
        uint mantissaLow : 16;
        uint mantissaHigh : 7;
        uint exponent : 8;
        bool isNegative : 1;
    };
} Float;

AvmString AvmStringFromFloat2(float value)
{
    Float f = {.value = value};

    int8_t exponent = f.exponent - 127; // Biased exponent.

    if (exponent > 18)
    {
        // Too big.
        return AvmStringFrom("inf");
    }

    if (exponent < -3)
    {
        // Too small.
        return AvmStringFrom("0");
    }

    AvmString s = AvmStringNew(8);

    if (f.isNegative)
    {
        AvmStringPushChar(&s, '-');
    }

    ulong mantissa = f.mantissaLow;
    mantissa += ((ulong)f.mantissaHigh << 16);
    mantissa += 0x00800000;

    AvmString intPart = AvmStringFromInt(mantissa >> (23 - exponent));
    AvmStringPushString(&s, &intPart);
    AvmObjectDestroy(&intPart);

    AvmStringPushChar(&s, '.');

    switch (0x7 & (mantissa >> (20 - exponent)))
    {
    case 0:
        AvmStringPushStr(&s, "000");
        break;
    case 1:
        AvmStringPushStr(&s, "125");
        break;
    case 2:
        AvmStringPushStr(&s, "250");
        break;
    case 3:
        AvmStringPushStr(&s, "375");
        break;
    case 4:
        AvmStringPushStr(&s, "500");
        break;
    case 5:
        AvmStringPushStr(&s, "625");
        break;
    case 6:
        AvmStringPushStr(&s, "750");
        break;
    case 7:
        AvmStringPushStr(&s, "875");
        break;
    }

    post
    {
        assert(s._buffer != NULL);
        assert(s._length != 0);
        assert(s._capacity != 0);
    }

    return s;
}

AvmString AvmStringFromFloat(double value)
{
    uint length = snprintf(NULL, 0, "%lf", value);
    AvmString s = AvmStringNew(length);
    char* buffer = AvmStringGetBuffer(&s);
    snprintf(buffer, length + 1, "%lf", value);
    AvmStringUnsafeSetLength(&s, length);

    post
    {
        assert(s._buffer != NULL);
        assert(s._length != 0);
        assert(s._capacity != 0);
    }

    return s;
}

AvmString AvmStringRepeat(str contents, uint count)
{
    pre
    {
        assert(contents != NULL);
    }

    if (count == 0)
    {
        return AvmStringNew(0);
    }

    if (count == 1)
    {
        return AvmStringFrom(contents);
    }

    const uint length = strlen(contents);

    if (length == 0)
    {
        return AvmStringNew(0);
    }

    AvmString self = AvmStringRepeatChars(length, contents, count);

    post
    {
        assert(self._buffer != NULL);
        assert(self._length != 0);
        assert(self._capacity == length * count * AVM_STRING_GROWTH_FACTOR);
    }

    return self;
}

AvmString AvmStringRepeatChars(uint length, str contents, uint count)
{
    pre
    {
        assert(contents != NULL);
    }

    if (length == 0 || count == 0)
    {
        return AvmStringNew(0);
    }

    // Allocate all the memory upfront, + some extra to avoid reallocations.
    AvmString self = AvmStringNew(length * count * AVM_STRING_GROWTH_FACTOR);

    for (uint i = 0; i < count; i++)
    {
        AvmStringPushChars(&self, length, contents);
    }

    post
    {
        assert(self._buffer != NULL);
        assert(self._length != 0);
        assert(self._capacity == length * count * AVM_STRING_GROWTH_FACTOR);
    }

    return self;
}

//
// Accessors.
//

uint AvmStringGetLength(const AvmString* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_length;
}

weakptr(char) AvmStringGetBuffer(const AvmString* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_buffer;
}

uint AvmStringGetCapacity(const AvmString* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_capacity;
}

bool AvmStringIsEmpty(const AvmString* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_length == 0;
}

char AvmStringCharAt(const AvmString* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    if (index < self->_length)
    {
        return self->_buffer[index];
    }

    AvmThrowError(RangeError);
}

//
// ForEach and overloads.
//

void AvmStringForEach(const AvmString* self, void (*function)(char))
{
    pre
    {
        assert(self != NULL);
        assert(function != NULL);
    }

    for (uint i = 0; i < self->_length; i++)
    {
        function(self->_buffer[i]);
    }
}

void AvmStringForEachEx(const AvmString* self, void (*function)(char, uint))
{
    pre
    {
        assert(self != NULL);
        assert(function != NULL);
    }

    for (uint i = 0; i < self->_length; i++)
    {
        function(self->_buffer[i], i);
    }
}

//
// Map and overloads.
//

void AvmStringMap(const AvmString* self, char (*function)(char))
{
    pre
    {
        assert(self != NULL);
        assert(function != NULL);
    }

    for (uint i = 0; i < self->_length; i++)
    {
        self->_buffer[i] = function(self->_buffer[i]);
    }
}

void AvmStringMapEx(const AvmString* self, char (*function)(char, uint))
{
    pre
    {
        assert(self != NULL);
        assert(function != NULL);
    }

    for (uint i = 0; i < self->_length; i++)
    {
        self->_buffer[i] = function(self->_buffer[i], i);
    }
}

void AvmStringMapCompat(const AvmString* self, int (*function)(int))
{
    pre
    {
        assert(self != NULL);
        assert(function != NULL);
    }

    for (uint i = 0; i < self->_length; i++)
    {
        self->_buffer[i] = (char)function((int)self->_buffer[i]);
    }
}

//
// Push and overloads.
//

void AvmStringPushChar(AvmString* self, char character)
{
    pre
    {
        assert(self != NULL);
    }

    AvmStringEnsureCapacity(self, 1);
    self->_buffer[self->_length] = character;
    self->_length++;

    post
    {
        assert(self->_capacity >= 1);
        assert(self->_length >= 1);
        assert(self->_buffer != NULL);
        assert(self->_buffer[self->_length - 1] == character);
    }
}

void AvmStringPushStr(AvmString* self, str contents)
{
    pre
    {
        assert(self != NULL);
        assert(contents != NULL);
    }

    const uint length = strlen(contents);
    AvmStringPushChars(self, length, contents);

    post
    {
        assert(self->_capacity >= length);
        assert(self->_length >= length);
        assert(self->_buffer != NULL);
        assert(self->_buffer[self->_length - 1] == contents[length - 1]);
    }
}

void AvmStringPushChars(AvmString* self, uint length, str contents)
{
    pre
    {
        assert(self != NULL);
        assert(contents != NULL);
    }

    if (length == 0)
    {
        return;
    }

    AvmStringEnsureCapacity(self, length);

    byte* const source = (byte*)contents;
    byte* const dest = (byte*)&self->_buffer[self->_length];

    memcpy(dest, source, length);

    // Don't forget to increase the length.
    self->_length += length;

    post
    {
        assert(self->_capacity >= length);
        assert(self->_length >= length);
        assert(self->_buffer != NULL);
    }
}

void AvmStringPushString(AvmString* self, const AvmString* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    if (other->_length == 0)
    {
        return;
    }

    AvmStringPushChars(self, other->_length, other->_buffer);

    post
    {
        assert(self->_capacity >= other->_length);
        assert(self->_length >= other->_length);
        assert(self->_buffer != NULL);
    }
}

//
// IndexOf, Find and overloads.
//

uint AvmStringIndexOf(const AvmString* self, char character)
{
    pre
    {
        assert(self != NULL);
    }

    for (uint i = 0; i < self->_length; i++)
    {
        if (self->_buffer[i] == character)
        {
            return i;
        }
    }

    return AvmInvalid;
}

uint AvmStringLastIndexOf(const AvmString* self, char character)
{
    pre
    {
        assert(self != NULL);
    }

    // Same as IndexOf but we loop in reverse.
    for (uint i = self->_length; i > 0; i--)
    {
        if (self->_buffer[i - 1] == character)
        {
            return i - 1;
        }
    }

    return AvmInvalid;
}

// TODO: Implement Find for AvmString and Chars.

uint AvmStringFind(const AvmString* self, str substring)
{
    pre
    {
        assert(self != NULL);
        assert(substring != NULL);
    }

    char* c = strstr(self->_buffer, substring);

    if (c == NULL)
    {
        return AvmInvalid;
    }

    return (uint)(c - self->_buffer);
}

uint AvmStringFindLast(const AvmString* self, str substring)
{
    pre
    {
        assert(self != NULL);
        assert(substring != NULL);
    }

    uint length = strlen(substring);

    if (length > self->_length)
    {
        return AvmInvalid;
    }

    for (char* end = self->_buffer + self->_length - length;
         end != self->_buffer;
         end--)
    {
        if (strncmp(end, substring, length) == 0)
        {
            return end - self->_buffer;
        }
    }

    return AvmInvalid;
}

//
// Replace and overloads.
//

uint AvmStringReplace(const AvmString* self,
                      char oldCharacter,
                      char newCharacter)
{
    pre
    {
        assert(self != NULL);
    }

    for (uint i = 0; i < self->_length; i++)
    {
        if (self->_buffer[i] == oldCharacter)
        {
            self->_buffer[i] = newCharacter;
            return i;
        }
    }

    return AvmInvalid;
}

uint AvmStringReplaceN(const AvmString* self,
                       uint count,
                       char oldCharacter,
                       char newCharacter)
{
    pre
    {
        assert(self != NULL);
    }

    uint realCount = 0;

    for (uint i = 0; i < self->_length && realCount < count; i++)
    {
        if (self->_buffer[i] == oldCharacter)
        {
            self->_buffer[i] = newCharacter;
            realCount++;
        }
    }

    return realCount;
}

uint AvmStringReplaceLastN(const AvmString* self,
                           uint count,
                           char oldCharacter,
                           char newCharacter)
{
    pre
    {
        assert(self != NULL);
    }

    uint realCount = 0;

    for (uint i = self->_length - 1; i + 1 > 0 && realCount < count; i--)
    {
        if (self->_buffer[i] == oldCharacter)
        {
            self->_buffer[i] = newCharacter;
            realCount++;
        }
    }

    return realCount;
}

uint AvmStringReplaceLast(const AvmString* self,
                          char oldCharacter,
                          char newCharacter)
{
    pre
    {
        assert(self != NULL);
    }

    for (uint i = self->_length - 1; i + 1 > 0; i--)
    {
        if (self->_buffer[i] == oldCharacter)
        {
            self->_buffer[i] = newCharacter;
            return i;
        }
    }

    return AvmInvalid;
}

uint AvmStringReplaceAll(const AvmString* self,
                         char oldCharacter,
                         char newCharacter)
{
    pre
    {
        assert(self != NULL);
    }

    uint count = 0;

    for (uint i = 0; i < self->_length; i++)
    {
        if (self->_buffer[i] == oldCharacter)
        {
            self->_buffer[i] = newCharacter;
            count++;
        }
    }

    return count;
}

//
// String manipulations.
//

void AvmStringReverse(const AvmString* self)
{
    pre
    {
        assert(self != NULL);
    }

    char* start = AvmStringGetBuffer(self);
    char* end = AvmStringGetBuffer(self) + self->_length - 1;

    for (char temp = 0; start < end; start++, end--)
    {
        temp = *start;
        *start = *end;
        *end = temp;
    }
}

void AvmStringToUpper(const AvmString* self)
{
    AvmStringMapCompat(self, toupper);
}

void AvmStringToLower(const AvmString* self)
{
    AvmStringMapCompat(self, tolower);
}

void AvmStringClear(AvmString* self)
{
    pre
    {
        assert(self != NULL);
    }

    self->_length = 0;
}

void AvmStringErase(AvmString* self)
{
    pre
    {
        assert(self != NULL);
    }

    memset(self->_buffer, 0, self->_length);
    self->_length = 0;
}

//
// Unsafe functions.
//

void AvmStringUnsafeSetLength(AvmString* self, uint length)
{
    pre
    {
        assert(self != NULL);
    }

    self->_length = length;
}

void AvmStringUnsafeDestruct(const AvmString* self,
                             uint* capacity,
                             uint* length,
                             char** buffer)
{
    pre
    {
        assert(self != NULL);
    }

    if (capacity != NULL)
    {
        *capacity = self->_capacity;
    }

    if (length != NULL)
    {
        *length = self->_length;
    }

    if (buffer != NULL)
    {
        *buffer = self->_buffer;
    }
}

AvmString AvmStringUnsafeFromRaw(uint capacity, uint length, char* buffer)
{
    return (AvmString){
        ._buffer = buffer,
        ._capacity = capacity,
        ._length = length,
        ._type = typeid(AvmString),
    };
}

//
// Contains and overloads.
//

bool AvmStringContainsChar(const AvmString* self, char character)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringIndexOf(self, character) != AvmInvalid;
}

bool AvmStringContainsStr(const AvmString* self, str contents)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFind(self, contents) != AvmInvalid;
}

//
// StartsWith and overloads.
//

bool AvmStringStartsWithChar(const AvmString* self, char character)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_buffer[0] == character;
}

bool AvmStringStartsWithChars(const AvmString* self, uint length, str contents)
{
    pre
    {
        assert(self != NULL);
    }

    if (self->_length < length)
    {
        return false;
    }

    return strncmp(self->_buffer, contents, length) == 0;
}

bool AvmStringStartsWithStr(const AvmString* self, str contents)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringStartsWithChars(self, strlen(contents), contents);
}

bool AvmStringStartsWithString(const AvmString* self, const AvmString* contents)
{
    pre
    {
        assert(self != NULL);
    }
    return AvmStringStartsWithChars(self, contents->_length, contents->_buffer);
}

//
// EndsWith and overloads.
//

bool AvmStringEndsWithChar(const AvmString* self, char character)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_buffer[self->_length - 1] == character;
}

bool AvmStringEndsWithStr(const AvmString* self, str contents)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringEndsWithChars(self, strlen(contents), contents);
}

bool AvmStringEndsWithChars(const AvmString* self, uint length, str contents)
{
    pre
    {
        assert(self != NULL);
    }

    if (self->_length < length)
    {
        return false;
    }

    uint index = self->_length - length;

    return strncmp(self->_buffer + index, contents, length) == 0;
}

bool AvmStringEndsWithString(const AvmString* self, const AvmString* contents)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringEndsWithChars(self, contents->_length, contents->_buffer);
}

void AvmStringPushInt(AvmString* self, _long value)
{
    pre
    {
        assert(self != NULL);
    }

    AvmString temp = AvmStringFromInt(value);
    AvmStringPushString(self, &temp);
    AvmObjectDestroy(&temp);
}

void AvmStringPushUint(AvmString* self, ulong value, AvmNumericBase numericBase)
{
    pre
    {
        assert(self != NULL);
    }

    AvmString temp = AvmStringFromUint(value, numericBase);
    switch (numericBase)
    {
    case NumericBaseBinary:
        AvmStringPushStr(self, AVM_FMT_BINARY_PREFIX);
        break;
    case NumericBaseOctal:
        AvmStringPushStr(self, AVM_FMT_OCTAL_PREFIX);
        break;
    case NumericBaseHex:
        AvmStringPushStr(self, AVM_FMT_HEX_PREFIX);
        break;
    default:
        break;
    }
    AvmStringPushString(self, &temp);
    AvmObjectDestroy(&temp);
}

void AvmStringPushFloat(AvmString* self, double value, AvmFloatRepr repr)
{
    pre
    {
        assert(self != NULL);
    }

    switch (repr)
    {
    case FloatReprSimple: {
        AvmString temp = AvmStringFromFloat(value);
        AvmStringPushString(self, &temp);
        AvmObjectDestroy(&temp);
        break;
    }
    case FloatReprScientific: {
        char buffer[AVM_FLOAT_BUFFER_SIZE] = {0};
        snprintf(buffer, AVM_FLOAT_BUFFER_SIZE, "%le", value);
        AvmStringPushStr(self, buffer);
        break;
    }
    case FloatReprAuto: {
        char buffer[AVM_FLOAT_BUFFER_SIZE] = {0};
        snprintf(buffer, AVM_FLOAT_BUFFER_SIZE, "%lg", value);
        AvmStringPushStr(self, buffer);
    }
    default:
        // TODO
        break;
    }
}

void AvmStringPushValue(AvmString* self, object value)
{
    pre
    {
        assert(self != NULL);
    }

    AvmString temp = AvmObjectToString(value);
    AvmStringPushString(self, &temp);
    AvmObjectDestroy(&temp);
}

//
// AvmStringFormat, AvmStringFormatV
//

static void Format(char c, AvmString* string, va_list args)
{
    switch (c)
    {
#ifdef AVM_HAVE_UCHAR_H
    case AVM_FMT_UNICODE:
        AvmStringPushStr(string, AVM_FMT_UNICODE_PREFIX);
        AvmStringPushUint(string, va_arg(args, char32_t), NumericBaseDecimal);
        break;
#endif
    case AVM_FMT_INT_SIZE:
    case AVM_FMT_INT_UNSIGNED:
        AvmStringPushUint(string, va_arg(args, ulong), NumericBaseDecimal);
        break;
    case AVM_FMT_INT_DECIMAL:
        AvmStringPushInt(string, va_arg(args, _long));
        break;
    case AVM_FMT_INT_OCTAL:
        AvmStringPushUint(string, va_arg(args, ulong), NumericBaseOctal);
        break;
    case AVM_FMT_POINTER:
    case AVM_FMT_INT_HEX:
        AvmStringPushUint(string, va_arg(args, ulong), NumericBaseHex);
        break;
    case AVM_FMT_INT_BINARY:
        AvmStringPushUint(string, va_arg(args, ulong), NumericBaseBinary);
        break;
    case AVM_FMT_FLOAT:
        AvmStringPushFloat(string, va_arg(args, double), FloatReprSimple);
        break;
    case AVM_FMT_FLOAT_EXP:
        AvmStringPushFloat(string, va_arg(args, double), FloatReprScientific);
        break;
    case AVM_FMT_FLOAT_AUTO:
        AvmStringPushFloat(string, va_arg(args, double), FloatReprAuto);
        break;
    case AVM_FMT_CHAR:
        AvmStringPushChar(string, (char)va_arg(args, int));
        break;
    case AVM_FMT_STRING:
        AvmStringPushStr(string, va_arg(args, char*));
        break;
    case AVM_FMT_BOOL:
        AvmStringPushStr(
            string, (bool)va_arg(args, uint) ? AVM_FMT_TRUE : AVM_FMT_FALSE);
        break;
    case AVM_FMT_TYPE:
        AvmStringPushStr(
            string, AvmTypeGetName(AvmObjectGetType(va_arg(args, object))));
        break;
    case AVM_FMT_SIZE:
        AvmStringPushUint(
            string,
            AvmTypeGetSize(AvmObjectGetType(va_arg(args, object))),
            NumericBaseDecimal);
        break;
    case AVM_FMT_VALUE:
        AvmStringPushValue(string, va_arg(args, object));
        break;
    default:
        AvmStringPushChar(string, c);
        break;
    }
}

AvmString AvmStringFormat(str format, ...)
{
    pre
    {
        assert(format != NULL);
    }

    va_list args;
    va_start(args, format);
    AvmString s = AvmStringFormatV(format, args);
    va_end(args);
    return s;
}

AvmString AvmStringFormatV(str format, va_list args)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
    }

    AvmString s = AvmStringNew(8);

    for (uint i = 0; format[i] != '\0'; i++)
    {
        if (format[i] != '%')
        {
            AvmStringPushChar(&s, format[i]);
            continue;
        }

        i++;

        Format(format[i], &s, args);
    }

    return s;
}

//
// AvmStringParse, AvmStringParseV
//

static void SkipWord(char* buffer, uint* index)
{
    while (buffer[*index] != ' ' && buffer[*index] != '\0')
    {
        (*index)++;
    }
}

static void ParseUint(uint* index,
                      char* buffer,
                      ulong* ptr,
                      AvmNumericBase numericBase)
{
    char* start = &buffer[*index];
    SkipWord(buffer, index);
    char* end = &buffer[*index];
    *ptr = strtoull(start, &end, numericBase);
}

static void ParseInt(uint* index, char* buffer, _long* ptr)
{
    char* start = &buffer[*index];
    SkipWord(buffer, index);
    char* end = &buffer[*index];
    *ptr = strtoll(start, &end, 10);
}

static void ParseBool(uint* index, char* buffer, bool* ptr)
{
    *ptr = strncmp(&buffer[*index], AVM_FMT_TRUE, 4) == 0;
    SkipWord(buffer, index);
}

static void ParseStr(uint* index, char* buffer, char* ptr, uint capacity)
{
    uint start = *index;
    SkipWord(buffer, index);

    uint length = *index - start;

    memcpy(ptr, &buffer[start], length);

    if (length < capacity)
    {
        ptr[length] = '\0';
    }
    else
    {
        ptr[capacity - 1] = '\0';
    }
}

static void ParseChar(uint* index, char* buffer, char* ptr)
{
    *ptr = buffer[*index];
    (*index)++;
}

static void Parse(char c, uint* index, char* buffer, va_list args)
{
    switch (c)
    {
    case AVM_FMT_CHAR:
        ParseChar(index, buffer, va_arg(args, char*));
        break;
    case AVM_FMT_BOOL:
        ParseBool(index, buffer, va_arg(args, bool*));
        break;
    case AVM_FMT_INT_DECIMAL:
        ParseInt(index, buffer, va_arg(args, _long*));
        break;
    case AVM_FMT_INT_BINARY:
        ParseUint(index, buffer, va_arg(args, ulong*), NumericBaseBinary);
        break;
    case AVM_FMT_INT_OCTAL:
        ParseUint(index, buffer, va_arg(args, ulong*), NumericBaseOctal);
        break;
    case AVM_FMT_INT_SIZE:
    case AVM_FMT_INT_UNSIGNED:
        ParseUint(index, buffer, va_arg(args, ulong*), NumericBaseDecimal);
        break;
    case AVM_FMT_POINTER:
    case AVM_FMT_INT_HEX:
        ParseUint(index, buffer, va_arg(args, ulong*), NumericBaseHex);
        break;
    case AVM_FMT_STRING:
        ParseStr(index, buffer, va_arg(args, char*), va_arg(args, uint));
    default:
        break;
    }
}

void AvmStringParse(const AvmString* self, str format, ...)
{
    pre
    {
        assert(self != NULL);
        assert(format != NULL);
    }

    va_list args;
    va_start(args, format);
    AvmStringParseV(self, format, args);
    va_end(args);
}

void AvmStringParseV(const AvmString* self, str format, va_list args)
{
    pre
    {
        assert(self != NULL);
        assert(format != NULL);
    }

    char* buffer = AvmStringGetBuffer(self);

    for (uint i = 0, j = 0; format[i] != '\0'; i++)
    {
        if (format[i] != '%')
        {
            continue;
        }

        i++;
        Parse(format[i], &j, buffer, args);
        j++;
    }
}
