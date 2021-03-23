#include "avium/string.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "avium/internal.h"

#define AVM_STRING_SIZE (sizeof(AvmType) + 2 * sizeof(size_t))

struct AvmString {
    AvmType type;
    size_t capacity;
    size_t length;
    char buffer[];
};

static AvmString AvmStringToString(AvmString self) {
    return AvmStringFrom(self->buffer);
}

static AvmString AvmStringClone(AvmString self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmString s = AvmStringNew(self->capacity);
    memcpy(s, self, self->capacity + AVM_STRING_SIZE + 1);
    return s;
}

TYPE(AvmString, [FUNC_GET_LENGTH] = (AvmFunction)AvmStringGetLength,
     [FUNC_GET_CAPACITY] = (AvmFunction)AvmStringGetCapacity,
     [FUNC_TO_STRING] = (AvmFunction)AvmStringToString,
     [FUNC_CLONE] = (AvmFunction)AvmStringClone);

AvmString AvmStringNew(size_t capacity) {
    AvmString s = malloc(AVM_STRING_SIZE + capacity + 1);
    s->capacity = capacity;
    s->type = GET_TYPE(AvmString);
    s->length = 0;
    return s;
}

AvmString AvmStringFrom(str contents) {
    if (contents == NULL) {
        AvmPanic(ContentsNullMsg);
    }

    size_t length = strlen(contents);
    AvmString s = AvmStringNew(length * AVM_STRING_GROWTH_FACTOR);
    s->length = length;
    memcpy(s->buffer, contents, length + 1);
    return s;
}

char* AvmStringAsPtr(AvmString self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->buffer;
}

size_t AvmStringGetLength(AvmString self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }
    return self->length;
}

size_t AvmStringGetCapacity(AvmString self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }
    return self->capacity;
}

AvmString AvmStringAppendChar(AvmString self, char character) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self->length == self->capacity) {
        self->capacity *= AVM_STRING_GROWTH_FACTOR;
        self = realloc(self, self->capacity + AVM_STRING_SIZE + 1);
    }

    self->buffer[self->length] = character;
    self->length++;
    self->buffer[self->length] = '\0';
    return self;
}

AvmString AvmStringAppend(AvmString self, str characters) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (characters == NULL) {
        AvmPanic("Parameter `characters` was `NULL`.");
    }

    size_t length = strlen(characters);
    self->length += length;

    if (self->length > self->capacity) {
        self->capacity *= AVM_STRING_GROWTH_FACTOR;
        self->capacity += length;
        self = realloc(self, self->capacity + AVM_STRING_SIZE + 1);
    }

    memcpy(&self->buffer[self->length - length], characters, length + 1);
    return self;
}

AvmString AvmStringConcat(AvmString self, AvmString other) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (other == NULL) {
        AvmPanic("Parameter `other` was `NULL`.");
    }

    size_t length = other->length;
    self->length += length;

    if (self->length > self->capacity) {
        self->capacity *= AVM_STRING_GROWTH_FACTOR;
        self->capacity += length;
        self = realloc(self, self->capacity + AVM_STRING_SIZE + 1);
    }

    memcpy(&self->buffer[self->length - length], other->buffer, length + 1);
    return self;
}

size_t AvmStringIndexOf(AvmString self, char character) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    for (size_t i = 0; i < self->length; i++) {
        if (self->buffer[i] == character) {
            return i;
        }
    }

    return AVM_STRING_NPOS;
}

size_t AvmStringLastIndexOf(AvmString self, char character) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    for (size_t i = self->length; i > 0; i--) {
        if (self->buffer[i - 1] == character) {
            return i - 1;
        }
    }

    return AVM_STRING_NPOS;
}

size_t AvmStringFind(AvmString self, str characters) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    char* c = strstr(self->buffer, characters);

    if (c == NULL) {
        return AVM_STRING_NPOS;
    }

    return (size_t)(c - self->buffer);
}

size_t AvmStringFindLast(AvmString self, str characters) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    size_t length = strlen(characters);

    if (length > self->length) {
        return AVM_STRING_NPOS;
    }

    for (char* end = self->buffer + self->length - length; end != self->buffer;
         end--) {
        if (strncmp(end, characters, length) == 0) {
            return end - self->buffer;
        }
    }

    return AVM_STRING_NPOS;
}

size_t AvmStringReplace(AvmString self, char oldCharacter, char newCharacter) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    size_t count = 0;

    for (size_t i = 0; i < self->length; i++) {
        if (self->buffer[i] == oldCharacter) {
            self->buffer[i] = newCharacter;
            count++;
        }
    }

    return count;
}

void AvmStringToUpper(AvmString self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    for (size_t i = 0; i < self->length; i++) {
        self->buffer[i] = (char)toupper(self->buffer[i]);
    }
}

void AvmStringToLower(AvmString self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    for (size_t i = 0; i < self->length; i++) {
        self->buffer[i] = (char)tolower(self->buffer[i]);
    }
}

void AvmStringUnsafeSetLength(AvmString self, size_t length) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    self->length = length;
}

char AvmStringCharAt(AvmString self, size_t index) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (index < self->length) {
        return self->buffer[index];
    }

    AvmPanic("Parameter `index` was greater than the string's length.");
}

void AvmStringReverse(AvmString self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    char* start = AvmStringAsPtr(self);
    char* end = AvmStringAsPtr(self) + self->length - 1;

    for (char temp = 0; start < end; start++, end--) {
        temp = *start;
        *start = *end;
        *end = temp;
    }
}
