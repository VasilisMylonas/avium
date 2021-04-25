#include "avium/string.h"

#include "avium/resources.h"
#include "avium/runtime.h"

#include <ctype.h>
#include <string.h>

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

#include "avium/fmt.h"
#include <stdio.h>

#ifdef AVM_HAVE_UCHAR_H
#    include <uchar.h>
#endif

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
                    AvmUtoa(va_arg(args, char32_t), NumericBaseHex);
                AvmStringPushStr(&s, AVM_FMT_UNICODE_PREFIX);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
#endif
            case AVM_FMT_INT_DECIMAL: {
                AvmString temp = AvmItoa(va_arg(args, _long));
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_INT_OCTAL: {
                AvmString temp = AvmUtoa(va_arg(args, ulong), NumericBaseOctal);
                AvmStringPushStr(&s, AVM_FMT_OCTAL_PREFIX);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_POINTER:
            case AVM_FMT_INT_HEX: {
                AvmString temp = AvmUtoa(va_arg(args, ulong), NumericBaseHex);
                AvmStringPushStr(&s, AVM_FMT_HEX_PREFIX);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_INT_BINARY: {
                AvmString temp =
                    AvmUtoa(va_arg(args, ulong), NumericBaseBinary);
                AvmStringPushStr(&s, AVM_FMT_BINARY_PREFIX);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_FLOAT: {
                AvmString temp = AvmFtoa(va_arg(args, double));
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
                    AvmUtoa(va_arg(args, ulong), NumericBaseDecimal);
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
                    AvmUtoa(AvmTypeGetSize(type), NumericBaseDecimal);
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
