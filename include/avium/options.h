#ifndef AVIUM_OPTIONS_H
#define AVIUM_OPTIONS_H

#include "avium/prologue.h"
#include "avium/array-list.h"

#include <stdlib.h>

typedef enum {
    OK_FLAG = 0,
    OK_INT,
    OK_FLOAT,
    OK_STRING,
} AvmOptionKind;

AVM_ARRAY_LIST_TYPE(str)

typedef struct AvmOptionParser AvmOptionParser;

AVMAPI AvmOptionParser* AvmOptionParserNew(int argc, str argv[]);
AVMAPI void AvmOptionParserAddStandardOptions(AvmOptionParser* self);

AVMAPI void AvmOptionParserAddOption(AvmOptionParser* self, str option,
                                     str description, AvmOptionKind kind);

AVMAPI void AvmOptionParserAddOptionEx(AvmOptionParser* self, str option,
                                       char shortOption, str description,
                                       AvmOptionKind kind);

AVMAPI void AvmOptionParserShowUsage(AvmOptionParser* self, str description);
AVMAPI AvmArrayList(str) AvmOptionParserParse(AvmOptionParser* self);

#endif  // AVIUM_OPTIONS_H
