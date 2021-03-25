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

AVM_CLASS(AvmOption, object, {
    str description;
    str longOption;
    AvmOptionKind kind;
    char shortOption;
});

AVM_ARRAY_LIST_TYPE(AvmOption)
AVM_ARRAY_LIST_TYPE(str)

AVM_CLASS(AvmOptionParser, object, {
    int argc;
    str* argv;
    AvmArrayList(AvmOption) options;
});

AVMAPI AvmOption AvmOptionFrom(str option, str description, AvmOptionKind kind);
AVMAPI AvmOption AvmOptionFromEx(str option, char shortOption, str description,
                                 AvmOptionKind kind);

AVMAPI void AvmOptionParserInit(AvmOptionParser* self, int argc, str argv[]);
AVMAPI void AvmOptionParserAddStandardOptions(AvmOptionParser* self);
AVMAPI void AvmOptionParserAddOption(AvmOptionParser* self, AvmOption option);
AVMAPI void AvmOptionParserShowUsage(AvmOptionParser* self, str description);
AVMAPI AvmArrayList(str) AvmOptionParserParse(AvmOptionParser* self);

#endif  // AVIUM_OPTIONS_H
