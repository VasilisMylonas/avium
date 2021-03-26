#ifndef AVIUM_OPTIONS_H
#define AVIUM_OPTIONS_H

#include "avium/prologue.h"
#include "avium/array-list.h"
#include "avium/value.h"

#include <stdlib.h>

AVM_ARRAY_LIST_TYPE(str)
AVM_ARRAY_LIST_TYPE(AvmValue)

typedef struct AvmOptionParser AvmOptionParser;

AVMAPI AvmOptionParser* AvmOptionParserNew(int argc, str argv[]);
AVMAPI void AvmOptionParserAddStandardOptions(AvmOptionParser* self);

AVMAPI void AvmOptionParserAddOption(AvmOptionParser* self, str option,
                                     str description, AvmValueKind kind);

AVMAPI void AvmOptionParserAddOptionEx(AvmOptionParser* self, str option,
                                       char shortOption, str description,
                                       AvmValueKind kind);

AVMAPI void AvmOptionParserShowUsage(AvmOptionParser* self, str description);

AVMAPI AvmArrayList(str) AvmOptionParserParseRaw(AvmOptionParser* self);
AVMAPI AvmArrayList(AvmValue) AvmOptionParserParse(AvmOptionParser* self);

#endif  // AVIUM_OPTIONS_H
