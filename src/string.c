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

AvmString AvmStringNew(size_t capacity) {
    return (AvmString){
        ._type = typeid(AvmString),
        ._length = 0,
        ._capacity = capacity,
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

    AvmString s = AvmStringNew(length * AVM_STRING_GROWTH_FACTOR);
    s._length = length;
    AvmMemCopy((byte*)contents, length, (byte*)s._buffer, length);
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

    AvmString s = AvmStringNew(length * count * AVM_STRING_GROWTH_FACTOR);
    for (size_t i = 0; i < count; i++) {
        AvmStringPushChars(&s, length, contents);
    }
    return s;
}

char* AvmStringAsPtr(AvmString* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_buffer;
}

size_t AvmStringGetLength(AvmString* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }
    return self->_length;
}

size_t AvmStringGetCapacity(AvmString* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }
    return self->_capacity;
}

void AvmStringForEach(AvmString* self, char (*function)(char)) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self == NULL) {
        AvmPanic(FunctionNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        self->_buffer[i] = function(self->_buffer[i]);
    }
}

void AvmStringForEachEx(AvmString* self, char (*function)(char, size_t)) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self == NULL) {
        AvmPanic(FunctionNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        self->_buffer[i] = function(self->_buffer[i], i);
    }
}

void AvmStringForEachCompat(AvmString* self, int (*function)(int)) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self == NULL) {
        AvmPanic(FunctionNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        self->_buffer[i] = (char)function((int)self->_buffer[i]);
    }
}

void AvmStringPushChar(AvmString* self, char character) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self->_length == self->_capacity) {
        self->_capacity *= AVM_STRING_GROWTH_FACTOR;
        self->_buffer = AvmRealloc(self->_buffer, self->_capacity + 1);
    }

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
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (contents == NULL) {
        AvmPanic(ContentsNullMsg);
    }

    self->_length += length;

    if (self->_length > self->_capacity) {
        self->_capacity *= AVM_STRING_GROWTH_FACTOR;
        self->_capacity += length;
        self->_buffer = AvmRealloc(self->_buffer, self->_capacity + 1);
    }

    AvmMemCopy((byte*)contents, length + 1,
               (byte*)&self->_buffer[self->_length - length], length + 1);
}

void AvmStringPushString(AvmString* self, AvmString* other) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (other == NULL) {
        AvmPanic(OtherNullMsg);
    }

    size_t length = other->_length;
    self->_length += length;

    if (self->_length > self->_capacity) {
        self->_capacity *= AVM_STRING_GROWTH_FACTOR;
        self->_capacity += length;
        self->_buffer = AvmRealloc(self->_buffer, self->_capacity + 1);
    }

    AvmMemCopy((byte*)other->_buffer, length + 1,
               (byte*)&self->_buffer[self->_length - length], length + 1);
}

size_t AvmStringIndexOf(AvmString* self, char character) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        if (self->_buffer[i] == character) {
            return i;
        }
    }

    return AvmInvalidIndex;
}

size_t AvmStringLastIndexOf(AvmString* self, char character) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    for (size_t i = self->_length; i > 0; i--) {
        if (self->_buffer[i - 1] == character) {
            return i - 1;
        }
    }

    return AvmInvalidIndex;
}

size_t AvmStringFind(AvmString* self, str substring) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (substring == NULL) {
        AvmPanic(SubstringNullMsg);
    }

    char* c = strstr(self->_buffer, substring);

    if (c == NULL) {
        return AvmInvalidIndex;
    }

    return (size_t)(c - self->_buffer);
}

size_t AvmStringFindLast(AvmString* self, str substring) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (substring == NULL) {
        AvmPanic(SubstringNullMsg);
    }

    size_t length = strlen(substring);

    if (length > self->_length) {
        return AvmInvalidIndex;
    }

    for (char* end = self->_buffer + self->_length - length;
         end != self->_buffer; end--) {
        if (strncmp(end, substring, length) == 0) {
            return end - self->_buffer;
        }
    }

    return AvmInvalidIndex;
}

size_t AvmStringReplace(AvmString* self, char oldCharacter, char newCharacter) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        if (self->_buffer[i] == oldCharacter) {
            self->_buffer[i] = newCharacter;
            return i;
        }
    }

    return AvmInvalidIndex;
}

size_t AvmStringReplaceN(AvmString* self, size_t count, char oldCharacter,
                         char newCharacter) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

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
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

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
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    for (size_t i = self->_length - 1; i + 1 > 0; i--) {
        if (self->_buffer[i] == oldCharacter) {
            self->_buffer[i] = newCharacter;
            return i;
        }
    }

    return AvmInvalidIndex;
}

size_t AvmStringReplaceAll(AvmString* self, char oldCharacter,
                           char newCharacter) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    size_t count = 0;

    for (size_t i = 0; i < self->_length; i++) {
        if (self->_buffer[i] == oldCharacter) {
            self->_buffer[i] = newCharacter;
            count++;
        }
    }

    return count;
}

void AvmStringUnsafeSetLength(AvmString* self, size_t length) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    self->_length = length;
}

AvmResult(char) AvmStringCharAt(AvmString* self, size_t index) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (index < self->_length) {
        return AvmSuccess(char)(self->_buffer[index]);
    }

    return AvmFailure(char)(AvmErrorFromOSCode(ErrorKindRange));
}

void AvmStringReverse(AvmString* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    char* start = AvmStringAsPtr(self);
    char* end = AvmStringAsPtr(self) + self->_length - 1;

    for (char temp = 0; start < end; start++, end--) {
        temp = *start;
        *start = *end;
        *end = temp;
    }
}

void AvmStringToUpper(AvmString* self) {
    AvmStringForEachCompat(self, toupper);
}

void AvmStringToLower(AvmString* self) {
    AvmStringForEachCompat(self, tolower);
}

AvmString AvmStringUnsafeFromRaw(size_t capacity, size_t length, char* buffer) {
    return (AvmString){
        ._buffer = buffer,
        ._capacity = capacity,
        ._length = length,
        ._type = typeid(AvmString),
    };
}

void AvmStringUnsafeDestruct(AvmString* self, size_t* capacity, size_t* length,
                             char** buffer) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

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

bool AvmStringIsEmpty(AvmString* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_length == 0;
}

void AvmStringClear(AvmString* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    self->_length = 0;
}

void AvmStringErase(AvmString* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    memset(self->_buffer, 0, self->_length);
    self->_length = 0;
}

bool AvmStringContainsChar(AvmString* self, char character) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return AvmStringIndexOf(self, character) != AvmInvalidIndex;
}

bool AvmStringContainsStr(AvmString* self, str contents) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return AvmStringFind(self, contents) != AvmInvalidIndex;
}

bool AvmStringStartsWithChar(AvmString* self, char character) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_buffer[0] == character;
}

bool AvmStringStartsWithChars(AvmString* self, size_t length, str contents) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self->_length < length) {
        return false;
    }

    return strncmp(self->_buffer, contents, length) == 0;
}

bool AvmStringStartsWithStr(AvmString* self, str contents) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return AvmStringStartsWithChars(self, strlen(contents), contents);
}

bool AvmStringStartsWithString(AvmString* self, AvmString* contents) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return AvmStringStartsWithChars(self, contents->_length, contents->_buffer);
}

bool AvmStringEndsWithChar(AvmString* self, char character) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_buffer[self->_length - 1] == character;
}

bool AvmStringEndsWithStr(AvmString* self, str contents) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return AvmStringEndsWithChars(self, strlen(contents), contents);
}

bool AvmStringEndsWithChars(AvmString* self, size_t length, str contents) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self->_length < length) {
        return false;
    }

    size_t index = self->_length - length;

    return strncmp(self->_buffer + index, contents, length) == 0;
}

bool AvmStringEndsWithString(AvmString* self, AvmString* contents) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return AvmStringEndsWithChars(self, contents->_length, contents->_buffer);
}
