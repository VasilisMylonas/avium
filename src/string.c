#include "avium/string.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "avium/internal.h"

#define AVM_STRING_SIZE (sizeof(Type) + 2 * sizeof(size_t))

static const char8_t* const selfNullMsg = "Parameter `self` was `NULL`.";
static const char8_t* const contentsNullMsg =
    "Parameter `contents` was `NULL`.";

struct AvmString {
    Type type;
    size_t capacity;
    size_t length;
    char8_t buffer[];
};

static AvmString AvmStringToString(AvmString self) {
    return AvmStringFrom(self->buffer);
}

static function_t AvmStringVTable[AVM_VTABLE_SIZE] = {
    [FUNC_GET_LENGTH] = (function_t)AvmStringGetLength,
    [FUNC_GET_CAPACITY] = (function_t)AvmStringGetCapacity,
    [FUNC_TO_STRING] = (function_t)AvmStringToString,
    [FUNC_CLONE] = (function_t)AvmStringClone,
};

static struct Type AvmStringType = {
    .size = AVM_STRING_SIZE,
    .name = "AvmString",
    .vptr = AvmStringVTable,
};

AvmString AvmStringNew(size_t capacity) {
    AvmString s = malloc(AVM_STRING_SIZE + capacity + 1);
    s->capacity = capacity;
    s->type = &AvmStringType;
    s->length = 0;
    return s;
}

AvmString AvmStringFrom(const char8_t* contents) {
    if (contents == NULL) {
        panic(contentsNullMsg);
    }

    size_t length = strlen(contents);
    AvmString s = AvmString(length * AVM_STRING_GROWTH_FACTOR);
    s->length = length;
    memcpy(s->buffer, contents, length + 1);
    return s;
}

char8_t* AvmStringAsPtr(AvmString self) {
    if (self == NULL) {
        panic(selfNullMsg);
    }

    return self->buffer;
}

size_t AvmStringGetLength(AvmString self) {
    if (self == NULL) {
        panic(selfNullMsg);
    }
    return self->length;
}

size_t AvmStringGetCapacity(AvmString self) {
    if (self == NULL) {
        panic(selfNullMsg);
    }
    return self->capacity;
}

AvmString AvmStringAppendChar(AvmString self, char8_t character) {
    if (self == NULL) {
        panic(selfNullMsg);
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

AvmString AvmStringAppend(AvmString self, const char8_t* characters) {
    if (self == NULL) {
        panic(selfNullMsg);
    }

    if (characters == NULL) {
        panic("Parameter `characters` was `NULL`.");
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
        panic(selfNullMsg);
    }

    if (other == NULL) {
        panic("Parameter `other` was `NULL`.");
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

size_t AvmStringIndexOf(AvmString self, char8_t character) {
    if (self == NULL) {
        panic(selfNullMsg);
    }

    for (size_t i = 0; i < self->length; i++) {
        if (self->buffer[i] == character) {
            return i;
        }
    }

    return AVM_STRING_NPOS;
}

size_t AvmStringLastIndexOf(AvmString self, char8_t character) {
    if (self == NULL) {
        panic(selfNullMsg);
    }

    for (size_t i = self->length; i > 0; i--) {
        if (self->buffer[i - 1] == character) {
            return i - 1;
        }
    }

    return AVM_STRING_NPOS;
}

size_t AvmStringFind(AvmString self, const char8_t* characters) {
    if (self == NULL) {
        panic(selfNullMsg);
    }

    char8_t* c = strstr(self->buffer, characters);

    if (c == NULL) {
        return AVM_STRING_NPOS;
    }

    return (size_t)(c - self->buffer);
}

size_t AvmStringFindLast(AvmString self, const char8_t* characters) {
    if (self == NULL) {
        panic(selfNullMsg);
    }

    size_t length = strlen(characters);

    for (char8_t* end = self->buffer + self->length - length;
         end != self->buffer; end--) {
        if (strncmp(end, characters, length) == 0) {
            return end - self->buffer;
        }
    }

    return AVM_STRING_NPOS;
}

size_t AvmStringReplace(AvmString self, char8_t oldCharacter,
                        char8_t newCharacter) {
    if (self == NULL) {
        panic(selfNullMsg);
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
        panic(selfNullMsg);
    }

    for (size_t i = 0; i < self->length; i++) {
        self->buffer[i] = (char8_t)toupper(self->buffer[i]);
    }
}

void AvmStringToLower(AvmString self) {
    if (self == NULL) {
        panic(selfNullMsg);
    }

    for (size_t i = 0; i < self->length; i++) {
        self->buffer[i] = (char8_t)tolower(self->buffer[i]);
    }
}

AvmString AvmStringClone(AvmString self) {
    if (self == NULL) {
        panic(selfNullMsg);
    }

    AvmString s = AvmString(self->capacity);
    memcpy(s, self, self->capacity + AVM_STRING_SIZE + 1);
    return s;
}

void AvmStringUnsafeSetLength(AvmString self, size_t length) {
    if (self == NULL) {
        panic(selfNullMsg);
    }

    self->length = length;
}

char8_t AvmStringCharAt(AvmString self, size_t index) {
    if (self == NULL) {
        panic(selfNullMsg);
    }

    if (index < self->length) {
        return self->buffer[index];
    }

    panic("Parameter `index` was greater than the string's length.");
}
