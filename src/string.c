#include "avium/string.h"

#include "avium/resources.h"
#include "avium/runtime.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifdef AVM_HAVE_UCHAR_H
#    include <uchar.h>
#endif

static AvmString AvmStringToString(AvmString* self) {
    return AvmStringFrom(self->_buffer);
}

static object AvmStringClone(AvmString* self) {
    AvmString s = AvmStringFrom(self->_buffer);
    AvmString* ret = AvmAlloc(sizeof(AvmString));
    AvmMemCopy((byte*)&s, sizeof(AvmString), (byte*)ret, sizeof(AvmString));
    return ret;
}

static void AvmStringDestroy(AvmString* self) { AvmDealloc(self->_buffer); }

AVM_TYPE(AvmString, {[FnEntryDtor] = (AvmFunction)AvmStringDestroy,
                     [FnEntryClone] = (AvmFunction)AvmStringClone,
                     [FnEntryToString] = (AvmFunction)AvmStringToString,
                     [FnEntryGetLength] = (AvmFunction)AvmStringGetLength,
                     [FnEntryGetCapacity] = (AvmFunction)AvmStringGetCapacity});

// Helpful macro for null checks.
#define AVM_SELF_NULL_CHECK()  \
    if (self == NULL) {        \
        AvmPanic(SelfNullMsg); \
    }

static void AvmStringReallocate(AvmString* self, size_t required) {
    AVM_SELF_NULL_CHECK();

    if (self->_length + required > self->_capacity) {
        // TODO: There may be more efficient ways of doing this
        self->_capacity *= AVM_STRING_GROWTH_FACTOR;
        self->_capacity += required;
        self->_buffer = AvmRealloc(self->_buffer, self->_capacity);
    }
}

//
// Constructors.
//

AvmString AvmStringNew(size_t capacity) {
    return (AvmString){
        ._type = typeid(AvmString),
        ._length = 0,
        ._capacity = capacity,
        // If capacity is 0 we should not allocate any memory.
        ._buffer = capacity == 0 ? NULL : AvmAlloc(capacity),
    };
}

AvmString AvmStringFrom(str contents) {
    if (contents == NULL) {
        AvmPanic(ContentsNullMsg);
    }

    return AvmStringFromChars(strlen(contents), contents);
}

AvmString AvmStringFromChars(size_t length, str contents) {
    if (length == 0) {
        return AvmStringNew(0);
    }

    if (contents == NULL) {
        AvmPanic(ContentsNullMsg);
    }

    // We allocate more memory upfront to reduce reallocations.
    AvmString s = AvmStringNew(length * AVM_STRING_GROWTH_FACTOR);
    AvmStringPushChars(&s, length, contents);

    return s;
}

AvmString AvmStringFromInt(_long value) {
    if (value == INTMAX_MIN) {
        return AvmStringFrom(LongMinRepr);
    }

    const bool isNegative = value < 0;
    if (isNegative) {
        value = -value;
    }

    AvmString s = AvmStringNew(8);

    size_t i = 0;
    for (; value != 0; i++, value /= 10) {
        _long r = value % 10;
        AvmStringPushChar(&s, '0' + r);
    }

    if (i == 0) {
        AvmStringPushChar(&s, '0');
        i++;
    }

    if (isNegative) {
        AvmStringPushChar(&s, '-');
        i++;
    }

    AvmStringReverse(&s);
    return s;
}

AvmString AvmStringFromUint(ulong value, AvmNumericBase numericBase) {
    switch (numericBase) {
        case NumericBaseBinary:
        case NumericBaseOctal:
        case NumericBaseDecimal:
        case NumericBaseHex:
            break;
        default:
            AvmPanic(NumericBaseOutOfRangeMsg);
    }

    AvmString s = AvmStringNew(8);

    size_t i = 0;
    for (; value != 0; i++, value /= numericBase) {
        _long r = value % numericBase;

        if (r >= 10) {
            AvmStringPushChar(&s, 'A' + (r - 10));
        } else {
            AvmStringPushChar(&s, '0' + r);
        }
    }

    if (i == 0) {
        AvmStringPushChar(&s, '0');
        i++;
    }

    AvmStringReverse(&s);
    return s;
}

typedef union {
    float value;

    struct {
        uint mantissaLow : 16;
        uint mantissaHigh : 7;
        uint exponent : 8;
        bool isNegative : 1;
    };
} Float;

AvmString AvmStringFromFloat2(float value) {
    Float f = {.value = value};

    int8_t exponent = f.exponent - 127;  // Biased exponent.

    if (exponent > 18) {
        // Too big.
        return AvmStringFrom("inf");
    }

    if (exponent < -3) {
        // Too small.
        return AvmStringFrom("0");
    }

    AvmString s = AvmStringNew(8);

    if (f.isNegative) {
        AvmStringPushChar(&s, '-');
    }

    ulong mantissa = f.mantissaLow;
    mantissa += ((ulong)f.mantissaHigh << 16);
    mantissa += 0x00800000;

    AvmString intPart = AvmStringFromInt(mantissa >> (23 - exponent));
    AvmStringPushString(&s, &intPart);
    AvmObjectDestroy(&intPart);

    AvmStringPushChar(&s, '.');

    switch (0x7 & (mantissa >> (20 - exponent))) {
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

    return s;
}

AvmString AvmStringFromFloat(double value) {
    size_t length = snprintf(NULL, 0, "%lf", value);
    AvmString s = AvmStringNew(length);
    char* buffer = AvmStringAsPtr(&s);
    snprintf(buffer, length + 1, "%lf", value);
    AvmStringUnsafeSetLength(&s, length);
    return s;
}

AvmString AvmStringRepeat(str contents, size_t count) {
    if (contents == NULL) {
        AvmPanic(ContentsNullMsg);
    }

    if (count == 0) {
        return AvmStringNew(0);
    }

    return AvmStringRepeatChars(strlen(contents), contents, count);
}

AvmString AvmStringRepeatChars(size_t length, str contents, size_t count) {
    if (contents == NULL) {
        AvmPanic(ContentsNullMsg);
    }

    if (length == 0 || count == 0) {
        return AvmStringNew(0);
    }

    // Allocate all the memory upfront, + some extra to avoid reallocations.
    AvmString s = AvmStringNew(length * count * AVM_STRING_GROWTH_FACTOR);

    for (size_t i = 0; i < count; i++) {
        AvmStringPushChars(&s, length, contents);
    }

    return s;
}

//
// Accessors.
//

size_t AvmStringGetLength(AvmString* self) {
    AVM_SELF_NULL_CHECK();

    return self->_length;
}

char* AvmStringAsPtr(AvmString* self) {
    AVM_SELF_NULL_CHECK();

    return self->_buffer;
}

size_t AvmStringGetCapacity(AvmString* self) {
    AVM_SELF_NULL_CHECK();

    return self->_capacity;
}

bool AvmStringIsEmpty(AvmString* self) {
    AVM_SELF_NULL_CHECK();

    return self->_length == 0;
}

char AvmStringCharAt(AvmString* self, size_t index, AvmError** error) {
    AVM_SELF_NULL_CHECK();

    if (index < self->_length) {
        return self->_buffer[index];
    }

    if (error != NULL) {
        *error = AvmErrorOfKind(ErrorKindRange);
    }

    return '\0';
}

//
// ForEach and overloads.
//

void AvmStringForEach(AvmString* self, void (*function)(char)) {
    AVM_SELF_NULL_CHECK();

    if (self == NULL) {
        AvmPanic(FunctionNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        function(self->_buffer[i]);
    }
}

void AvmStringForEachEx(AvmString* self, void (*function)(char, size_t)) {
    AVM_SELF_NULL_CHECK();

    if (self == NULL) {
        AvmPanic(FunctionNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        function(self->_buffer[i], i);
    }
}

//
// Map and overloads.
//

void AvmStringMap(AvmString* self, char (*function)(char)) {
    AVM_SELF_NULL_CHECK();

    if (self == NULL) {
        AvmPanic(FunctionNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        self->_buffer[i] = function(self->_buffer[i]);
    }
}

void AvmStringMapEx(AvmString* self, char (*function)(char, size_t)) {
    AVM_SELF_NULL_CHECK();

    if (self == NULL) {
        AvmPanic(FunctionNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        self->_buffer[i] = function(self->_buffer[i], i);
    }
}

void AvmStringMapCompat(AvmString* self, int (*function)(int)) {
    AVM_SELF_NULL_CHECK();

    if (self == NULL) {
        AvmPanic(FunctionNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        self->_buffer[i] = (char)function((int)self->_buffer[i]);
    }
}

//
// Push and overloads.
//

void AvmStringPushChar(AvmString* self, char character) {
    AVM_SELF_NULL_CHECK();

    AvmStringReallocate(self, 1);
    self->_buffer[self->_length] = character;
    self->_length++;
}

void AvmStringPushStr(AvmString* self, str contents) {
    if (contents == NULL) {
        AvmPanic(ContentsNullMsg);
    }

    AvmStringPushChars(self, strlen(contents), contents);
}

void AvmStringPushChars(AvmString* self, size_t length, str contents) {
    AVM_SELF_NULL_CHECK();

    if (length == 0) {
        return;
    }

    if (contents == NULL) {
        AvmPanic(ContentsNullMsg);
    }

    AvmStringReallocate(self, length);

    byte* const source = (byte*)contents;
    byte* const dest = (byte*)&self->_buffer[self->_length];

    AvmMemCopy(source, length, dest, self->_capacity);

    // Don't forget to increase the length.
    self->_length += length;
}

void AvmStringPushString(AvmString* self, AvmString* other) {
    AVM_SELF_NULL_CHECK();

    if (other == NULL) {
        AvmPanic(OtherNullMsg);
    }

    AvmStringPushChars(self, other->_length, other->_buffer);
}

//
// IndexOf, Find and overloads.
//

size_t AvmStringIndexOf(AvmString* self, char character) {
    AVM_SELF_NULL_CHECK();

    for (size_t i = 0; i < self->_length; i++) {
        if (self->_buffer[i] == character) {
            return i;
        }
    }

    return AvmInvalid;
}

size_t AvmStringLastIndexOf(AvmString* self, char character) {
    AVM_SELF_NULL_CHECK();

    // Same as IndexOf but we loop in reverse.
    for (size_t i = self->_length; i > 0; i--) {
        if (self->_buffer[i - 1] == character) {
            return i - 1;
        }
    }

    return AvmInvalid;
}

// TODO: Implement Find for AvmString and Chars.

size_t AvmStringFind(AvmString* self, str substring) {
    AVM_SELF_NULL_CHECK();

    if (substring == NULL) {
        AvmPanic(SubstringNullMsg);
    }

    char* c = strstr(self->_buffer, substring);

    if (c == NULL) {
        return AvmInvalid;
    }

    return (size_t)(c - self->_buffer);
}

size_t AvmStringFindLast(AvmString* self, str substring) {
    AVM_SELF_NULL_CHECK();

    if (substring == NULL) {
        AvmPanic(SubstringNullMsg);
    }

    size_t length = strlen(substring);

    if (length > self->_length) {
        return AvmInvalid;
    }

    for (char* end = self->_buffer + self->_length - length;
         end != self->_buffer; end--) {
        if (strncmp(end, substring, length) == 0) {
            return end - self->_buffer;
        }
    }

    return AvmInvalid;
}

//
// Replace and overloads.
//

size_t AvmStringReplace(AvmString* self, char oldCharacter, char newCharacter) {
    AVM_SELF_NULL_CHECK();

    for (size_t i = 0; i < self->_length; i++) {
        if (self->_buffer[i] == oldCharacter) {
            self->_buffer[i] = newCharacter;
            return i;
        }
    }

    return AvmInvalid;
}

size_t AvmStringReplaceN(AvmString* self, size_t count, char oldCharacter,
                         char newCharacter) {
    AVM_SELF_NULL_CHECK();

    size_t realCount = 0;

    for (size_t i = 0; i < self->_length && realCount < count; i++) {
        if (self->_buffer[i] == oldCharacter) {
            self->_buffer[i] = newCharacter;
            realCount++;
        }
    }

    return realCount;
}

size_t AvmStringReplaceLastN(AvmString* self, size_t count, char oldCharacter,
                             char newCharacter) {
    AVM_SELF_NULL_CHECK();

    size_t realCount = 0;

    for (size_t i = self->_length - 1; i + 1 > 0 && realCount < count; i--) {
        if (self->_buffer[i] == oldCharacter) {
            self->_buffer[i] = newCharacter;
            realCount++;
        }
    }

    return realCount;
}

size_t AvmStringReplaceLast(AvmString* self, char oldCharacter,
                            char newCharacter) {
    AVM_SELF_NULL_CHECK();

    for (size_t i = self->_length - 1; i + 1 > 0; i--) {
        if (self->_buffer[i] == oldCharacter) {
            self->_buffer[i] = newCharacter;
            return i;
        }
    }

    return AvmInvalid;
}

size_t AvmStringReplaceAll(AvmString* self, char oldCharacter,
                           char newCharacter) {
    AVM_SELF_NULL_CHECK();

    size_t count = 0;

    for (size_t i = 0; i < self->_length; i++) {
        if (self->_buffer[i] == oldCharacter) {
            self->_buffer[i] = newCharacter;
            count++;
        }
    }

    return count;
}

//
// String manipulations.
//

void AvmStringReverse(AvmString* self) {
    AVM_SELF_NULL_CHECK();

    char* start = AvmStringAsPtr(self);
    char* end = AvmStringAsPtr(self) + self->_length - 1;

    for (char temp = 0; start < end; start++, end--) {
        temp = *start;
        *start = *end;
        *end = temp;
    }
}

void AvmStringToUpper(AvmString* self) { AvmStringMapCompat(self, toupper); }

void AvmStringToLower(AvmString* self) { AvmStringMapCompat(self, tolower); }

void AvmStringClear(AvmString* self) {
    AVM_SELF_NULL_CHECK();

    self->_length = 0;
}

void AvmStringErase(AvmString* self) {
    AVM_SELF_NULL_CHECK();

    memset(self->_buffer, 0, self->_length);
    self->_length = 0;
}

//
// Unsafe functions.
//

void AvmStringUnsafeSetLength(AvmString* self, size_t length) {
    AVM_SELF_NULL_CHECK();

    self->_length = length;
}

void AvmStringUnsafeDestruct(AvmString* self, size_t* capacity, size_t* length,
                             char** buffer) {
    AVM_SELF_NULL_CHECK();

    if (capacity != NULL) {
        *capacity = self->_capacity;
    }

    if (length != NULL) {
        *length = self->_length;
    }

    if (buffer != NULL) {
        *buffer = self->_buffer;
    }
}

AvmString AvmStringUnsafeFromRaw(size_t capacity, size_t length, char* buffer) {
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

bool AvmStringContainsChar(AvmString* self, char character) {
    AVM_SELF_NULL_CHECK();

    return AvmStringIndexOf(self, character) != AvmInvalid;
}

bool AvmStringContainsStr(AvmString* self, str contents) {
    AVM_SELF_NULL_CHECK();

    return AvmStringFind(self, contents) != AvmInvalid;
}

//
// StartsWith and overloads.
//

bool AvmStringStartsWithChar(AvmString* self, char character) {
    AVM_SELF_NULL_CHECK();

    return self->_buffer[0] == character;
}

bool AvmStringStartsWithChars(AvmString* self, size_t length, str contents) {
    AVM_SELF_NULL_CHECK();

    if (self->_length < length) {
        return false;
    }

    return strncmp(self->_buffer, contents, length) == 0;
}

bool AvmStringStartsWithStr(AvmString* self, str contents) {
    AVM_SELF_NULL_CHECK();

    return AvmStringStartsWithChars(self, strlen(contents), contents);
}

bool AvmStringStartsWithString(AvmString* self, AvmString* contents) {
    AVM_SELF_NULL_CHECK();

    return AvmStringStartsWithChars(self, contents->_length, contents->_buffer);
}

//
// EndsWith and overloads.
//

bool AvmStringEndsWithChar(AvmString* self, char character) {
    AVM_SELF_NULL_CHECK();

    return self->_buffer[self->_length - 1] == character;
}

bool AvmStringEndsWithStr(AvmString* self, str contents) {
    AVM_SELF_NULL_CHECK();

    return AvmStringEndsWithChars(self, strlen(contents), contents);
}

bool AvmStringEndsWithChars(AvmString* self, size_t length, str contents) {
    AVM_SELF_NULL_CHECK();

    if (self->_length < length) {
        return false;
    }

    size_t index = self->_length - length;

    return strncmp(self->_buffer + index, contents, length) == 0;
}

bool AvmStringEndsWithString(AvmString* self, AvmString* contents) {
    AVM_SELF_NULL_CHECK();

    return AvmStringEndsWithChars(self, contents->_length, contents->_buffer);
}

AvmString AvmStringFormat(str format, ...) {
    va_list args;
    va_start(args, format);
    AvmString s = AvmStringFormatV(format, args);
    va_end(args);
    return s;
}

AvmString AvmStringFormatV(str format, va_list args) {
    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }

    if (args == NULL) {
        AvmPanic(ArgsNullMsg);
    }

    AvmString s = AvmStringNew(8);

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') {
            AvmStringPushChar(&s, format[i]);
            continue;
        }

        i++;

        switch (format[i]) {
#ifdef AVM_HAVE_UCHAR_H
            case AVM_FMT_UNICODE: {
                AvmString temp =
                    AvmStringFromUint(va_arg(args, char32_t), NumericBaseHex);
                AvmStringPushStr(&s, AVM_FMT_UNICODE_PREFIX);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
#endif
            case AVM_FMT_INT_DECIMAL: {
                AvmString temp = AvmStringFromInt(va_arg(args, _long));
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_INT_OCTAL: {
                AvmString temp =
                    AvmStringFromUint(va_arg(args, ulong), NumericBaseOctal);
                AvmStringPushStr(&s, AVM_FMT_OCTAL_PREFIX);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_POINTER:
            case AVM_FMT_INT_HEX: {
                AvmString temp =
                    AvmStringFromUint(va_arg(args, ulong), NumericBaseHex);
                AvmStringPushStr(&s, AVM_FMT_HEX_PREFIX);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_INT_BINARY: {
                AvmString temp =
                    AvmStringFromUint(va_arg(args, ulong), NumericBaseBinary);
                AvmStringPushStr(&s, AVM_FMT_BINARY_PREFIX);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_FLOAT: {
                AvmString temp = AvmStringFromFloat(va_arg(args, double));
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_FLOAT_EXP: {
                char buffer[AVM_FLOAT_BUFFER_SIZE] = {0};
                snprintf(buffer, AVM_FLOAT_BUFFER_SIZE, "%le",
                         va_arg(args, double));
                AvmStringPushStr(&s, buffer);
                break;
            }
            case AVM_FMT_FLOAT_AUTO: {
                char buffer[AVM_FLOAT_BUFFER_SIZE] = {0};
                snprintf(buffer, AVM_FLOAT_BUFFER_SIZE, "%lg",
                         va_arg(args, double));
                AvmStringPushStr(&s, buffer);
                break;
            }
            case AVM_FMT_INT_SIZE:
            case AVM_FMT_INT_UNSIGNED: {
                AvmString temp =
                    AvmStringFromUint(va_arg(args, ulong), NumericBaseDecimal);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_CHAR:
                AvmStringPushChar(&s, (char)va_arg(args, int));
                break;
            case AVM_FMT_STRING:
                AvmStringPushStr(&s, va_arg(args, char*));
                break;
            case AVM_FMT_BOOL:
                AvmStringPushStr(&s, (bool)va_arg(args, uint) ? AVM_FMT_TRUE
                                                              : AVM_FMT_FALSE);
                break;
            case AVM_FMT_TYPE: {
                const AvmType* type = AvmObjectGetType(va_arg(args, object));
                AvmStringPushStr(&s, AvmTypeGetName(type));
                break;
            }
            case AVM_FMT_SIZE: {
                const AvmType* type = AvmObjectGetType(va_arg(args, object));
                AvmString temp =
                    AvmStringFromUint(AvmTypeGetSize(type), NumericBaseDecimal);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_VALUE: {
                AvmString temp = AvmObjectToString(va_arg(args, object));
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            default:
                AvmStringPushChar(&s, format[i]);
                break;
        }
    }

    return s;
}

static void SkipWord(char* buffer, size_t* index) {
    while (buffer[*index] != ' ' && buffer[*index] != '\0') {
        (*index)++;
    }
}

#define UINT_CASE(base)       \
    char* start = &buffer[j]; \
    SkipWord(buffer, &j);     \
    char* end = &buffer[j];   \
    *((ulong*)va_arg(args, ulong*)) = strtoull(start, &end, base)

void AvmStringParse(AvmString* self, str format, ...) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }

    va_list args;
    va_start(args, format);
    AvmStringParseV(self, format, args);
    va_end(args);
}

void AvmStringParseV(AvmString* self, str format, va_list args) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }

    char* buffer = AvmStringAsPtr(self);

    for (size_t i = 0, j = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') {
            continue;
        }

        i++;

        switch (format[i]) {
            case AVM_FMT_CHAR: {
                char* c = va_arg(args, char*);
                *c = buffer[j];
                j++;
                break;
            }
            case AVM_FMT_BOOL: {
                *((bool*)va_arg(args, bool*)) =
                    strncmp(&buffer[j], AVM_FMT_TRUE, 4) == 0;
                SkipWord(buffer, &j);
                break;
            }
            case AVM_FMT_INT_DECIMAL: {
                char* start = &buffer[j];
                SkipWord(buffer, &j);
                char* end = &buffer[j];
                *((_long*)va_arg(args, _long*)) = strtoll(start, &end, 10);
                break;
            }
            case AVM_FMT_INT_BINARY: {
                UINT_CASE(2);
                break;
            }
            case AVM_FMT_INT_OCTAL: {
                UINT_CASE(8);
                break;
            }
            case AVM_FMT_INT_SIZE:
            case AVM_FMT_INT_UNSIGNED: {
                UINT_CASE(10);
                break;
            }
            case AVM_FMT_POINTER:
            case AVM_FMT_INT_HEX: {
                UINT_CASE(16);
                break;
            }
            case AVM_FMT_STRING: {
                char* s = va_arg(args, char*);
                size_t start = j;
                SkipWord(buffer, &j);

                size_t capacity = va_arg(args, size_t);
                size_t length = j - start;

                AvmMemCopy((byte*)&buffer[start], length, (byte*)s, capacity);

                if (length < capacity) {
                    s[length] = '\0';
                } else {
                    s[capacity - 1] = '\0';
                }
            }
            default:
                break;
        }

        j++;
    }
}
