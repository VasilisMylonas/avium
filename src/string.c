#include "avium/string.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "avium/internal.h"

static AvmString AvmStringToString(AvmString* self) {
    return AvmStringFrom(self->_buffer);
}

static AvmString AvmStringClone(AvmString* self) {
    return AvmStringFrom(self->_buffer);
}

static void AvmStringDestroy(AvmString self) { free(self._buffer); }

TYPE(AvmString, [FUNC_DTOR] = (AvmFunction)AvmStringDestroy,
     [FUNC_CLONE] = (AvmFunction)AvmStringClone,
     [FUNC_TO_STRING] = (AvmFunction)AvmStringToString,
     [FUNC_GET_LENGTH] = (AvmFunction)AvmStringGetLength,
     [FUNC_GET_CAPACITY] = (AvmFunction)AvmStringGetCapacity);

AvmString AvmStringNew(size_t capacity) {
    return (AvmString){
        ._type = GET_TYPE(AvmString),
        ._length = 0,
        ._capacity = capacity,
        ._buffer = capacity == 0 ? NULL : malloc(capacity),
    };
}

AvmString AvmStringFrom(str contents) {
    if (contents == NULL) {
        AvmPanic(ContentsNullMsg);
    }

    size_t length = strlen(contents);
    AvmString s = AvmStringNew(length * AVM_STRING_GROWTH_FACTOR);
    s._length = length;
    memcpy(s._buffer, contents, length + 1);
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

void AvmStringToUpper(AvmString* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        self->_buffer[i] = (char)toupper(self->_buffer[i]);
    }
}

void AvmStringToLower(AvmString* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        self->_buffer[i] = (char)tolower(self->_buffer[i]);
    }
}

void AvmStringPushChar(AvmString* self, char character) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self->_length == self->_capacity) {
        self->_capacity *= AVM_STRING_GROWTH_FACTOR;
        self->_buffer = realloc(self->_buffer, self->_capacity + 1);
    }

    self->_buffer[self->_length] = character;
    self->_length++;
    self->_buffer[self->_length] = '\0';
}

void AvmStringPushStr(AvmString* self, str contents) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (contents == NULL) {
        AvmPanic("Parameter `contents` was `NULL`.");
    }

    size_t length = strlen(contents);
    self->_length += length;

    if (self->_length > self->_capacity) {
        self->_capacity *= AVM_STRING_GROWTH_FACTOR;
        self->_capacity += length;
        self->_buffer = realloc(self->_buffer, self->_capacity + 1);
    }

    memcpy(&self->_buffer[self->_length - length], contents, length + 1);
}

void AvmStringPushString(AvmString* self, AvmString* other) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (other == NULL) {
        AvmPanic("Parameter `other` was `NULL`.");
    }

    size_t length = other->_length;
    self->_length += length;

    if (self->_length > self->_capacity) {
        self->_capacity *= AVM_STRING_GROWTH_FACTOR;
        self->_capacity += length;
        self = realloc(self, self->_capacity + 1);
    }

    memcpy(&self->_buffer[self->_length - length], other->_buffer, length + 1);
}

AvmOptional(size_t) AvmStringIndexOf(AvmString* self, char character) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    for (size_t i = 0; i < self->_length; i++) {
        if (self->_buffer[i] == character) {
            return AvmSome(size_t)(i);
        }
    }

    return AvmNone(size_t)();
}

AvmOptional(size_t) AvmStringLastIndexOf(AvmString* self, char character) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    for (size_t i = self->_length; i > 0; i--) {
        if (self->_buffer[i - 1] == character) {
            return AvmSome(size_t)(i - 1);
        }
    }

    return AvmNone(size_t)();
}

AvmOptional(size_t) AvmStringFind(AvmString* self, str substring) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (substring == NULL) {
        AvmPanic("Parameter `substring` was `NULL`.");
    }

    char* c = strstr(self->_buffer, substring);

    if (c == NULL) {
        return AvmNone(size_t)();
    }

    return AvmSome(size_t)((size_t)(c - self->_buffer));
}

AvmOptional(size_t) AvmStringFindLast(AvmString* self, str characters) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    size_t length = strlen(characters);

    if (length > self->_length) {
        return AvmNone(size_t)();
    }

    for (char* end = self->_buffer + self->_length - length;
         end != self->_buffer; end--) {
        if (strncmp(end, characters, length) == 0) {
            return AvmSome(size_t)(end - self->_buffer);
        }
    }

    return AvmNone(size_t)();
}

size_t AvmStringReplace(AvmString* self, char oldCharacter, char newCharacter) {
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

    return AvmFailure(char)(ErrorKindRange,
                            "The specified index was out of range.");
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
