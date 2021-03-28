#include "avium/options.h"
#include "avium/fmt.h"
#include "avium/string.h"

#include <string.h>
#include <stdlib.h>

typedef struct {
    str description;
    str longOption;
    AvmValueKind kind;
    char shortOption;
} AvmOption;

AVM_ARRAY_LIST_TYPE(AvmOption)

AVM_CLASS(AvmOptionParser, object, {
    int argc;
    str* argv;
    AvmArrayList(AvmOption) options;
});

static void AvmOptionParserDestroy(AvmOptionParser* self) {
    AvmObjectDestroy(&self->options);
}

AVM_TYPE(AvmOptionParser, {[FUNC_DTOR] = (AvmFunction)AvmOptionParserDestroy});

AvmOptionParser* AvmOptionParserNew(int argc, str argv[]) {
    AvmOptionParser* self = malloc(sizeof(AvmOptionParser));
    self->_type = AVM_GET_TYPE(AvmOptionParser);
    self->argc = argc;
    self->argv = argv;
    self->options = AvmArrayListNew(AvmOption)(4);
    return self;
}

void AvmOptionParserAddOption(AvmOptionParser* self, str option,
                              str description, AvmValueKind kind) {
    AvmOptionParserAddOptionEx(self, option, option[0], description, kind);
}

void AvmOptionParserAddOptionEx(AvmOptionParser* self, str option,
                                char shortOption, str description,
                                AvmValueKind kind) {
    AvmArrayListPush(AvmOption)(&self->options, (AvmOption){
                                                    .description = description,
                                                    .kind = kind,
                                                    .longOption = option,
                                                    .shortOption = shortOption,
                                                });
}

void AvmOptionParserAddStandardOptions(AvmOptionParser* self) {
    AvmOptionParserAddOption(
        self, "verbose",
        "Show extended information about what the program is doing.",
        ValueKindBool);

    AvmOptionParserAddOption(self, "help", "Show the help prompt.",
                             ValueKindBool);

    AvmOptionParserAddOptionEx(self, "version", 'V',
                               "Show version information.", ValueKindBool);
}

void AvmOptionParserShowUsage(AvmOptionParser* self, str description) {
    str s = strrchr(self->argv[0], '/') + 1;
    AvmPrintf("usage: %s ", s);

    const size_t length = AvmArrayListGetLength(AvmOption)(&self->options);

    for (size_t i = 0; i < length; i++) {
        AvmPrintf("[-%c", self->options._items[i].shortOption);

        if (self->options._items[i].kind != ValueKindBool) {
            AvmPrintf(" --%s", self->options._items[i].longOption);
        }

        AvmPrintf("] ");
    }

    AvmPrintf("\n\n%s\n\nOptional Arguments:\n", description);

    for (size_t i = 0; i < length; i++) {
        if (self->options._items[i].kind == ValueKindBool) {
            if (self->options._items[i].shortOption != 0) {
                AvmPrintf("  -%c, --%s - %s\n",
                          self->options._items[i].shortOption,
                          self->options._items[i].longOption,
                          self->options._items[i].description);
            } else {
                AvmPrintf("  --%s - %s\n", self->options._items[i].longOption,
                          self->options._items[i].description);
            }
        } else {
            if (self->options._items[i].shortOption != 0) {
                AvmString s = AvmStringFrom(self->options._items[i].longOption);
                AvmStringToUpper(&s);
                AvmPrintf("  -%c %v, --%s=%v - %s\n",
                          self->options._items[i].shortOption, &s,
                          self->options._items[i].longOption, &s,
                          self->options._items[i].description);
                AvmObjectDestroy(&s);
            } else {
                AvmString s = AvmStringFrom(self->options._items[i].longOption);
                AvmStringToUpper(&s);
                AvmPrintf("  --%s=%v - %s\n",
                          self->options._items[i].longOption, &s,
                          self->options._items[i].description);
                AvmObjectDestroy(&s);
            }
        }
    }
}

static bool IsShortOption(str arg, char shortOption) {
    return arg[0] == '-' && arg[1] == shortOption;
}

static bool IsLongOption(str arg, str longOption, size_t length) {
    return arg[0] == '-' && arg[1] == '-' &&
           strncmp(&arg[2], longOption, length) == 0;
}

static str HandleShortOption(str current, str next, AvmOption* option) {
    if (option->kind != ValueKindBool) {
        if (current[2] != '\0') {
            return &current[2];  // -ofile.txt
        }

        if (next != NULL) {
            return next;  // -o file.txt
        }
    }

    return "";  // -o
}

static str HandleLongOption(str current, str next, AvmOption* option,
                            size_t optionLength) {
    if (option->kind != ValueKindBool) {
        if (current[optionLength + 2] == '=') {
            // --output=file.txt
            return &current[optionLength + 3];
        }

        if (next != NULL) {
            // --output file.txt
            return next;
        }
    }

    return "";  // --output
}

static str AvmGetOption(size_t length, str args[], AvmOption* option) {
    size_t optionLength = strlen(option->longOption);

    // 1st entry is the program name.
    for (size_t i = 1; i < length; i++) {
        if (IsShortOption(args[i], option->shortOption)) {
            return HandleShortOption(args[i], args[i + 1], option);
        }

        if (IsLongOption(args[i], option->longOption, optionLength)) {
            return HandleLongOption(args[i], args[i + 1], option, optionLength);
        }
    }

    return NULL;
}

AvmArrayList(str) AvmOptionParserParseRaw(AvmOptionParser* self) {
    AvmArrayList(str) list =
        AvmArrayListNew(str)(AvmArrayListGetLength(AvmOption)(&self->options));

    for (size_t i = 0; i < AvmArrayListGetLength(AvmOption)(&self->options);
         i++) {
        AvmArrayListPush(str)(&list, AvmGetOption(self->argc, self->argv,
                                                  &self->options._items[i]));
    }

    return list;
}

AvmArrayList(AvmValue) AvmOptionParserParse(AvmOptionParser* self) {
    AvmArrayList(AvmValue) list = AvmArrayListNew(AvmValue)(
        AvmArrayListGetLength(AvmOption)(&self->options));

    for (size_t i = 0; i < AvmArrayListGetLength(AvmOption)(&self->options);
         i++) {
        str option =
            AvmGetOption(self->argc, self->argv, &self->options._items[i]);

        switch (self->options._items[i].kind) {
            case ValueKindFloat:
                AvmArrayListPush(AvmValue)(&list,
                                           AvmValueFromFloat(atof(option)));
                break;
            case ValueKindInt:
                AvmArrayListPush(AvmValue)(&list,
                                           AvmValueFromInt(atoll(option)));
                break;
            case ValueKindUInt:
                // AvmArrayListPush(AvmValue)(&list,
                //                            AvmValueFromUInt(ato ll(option)));
                break;
            case ValueKindStr:
                AvmArrayListPush(AvmValue)(&list, AvmValueFromStr(option));
                break;
            case ValueKindChar:
                AvmArrayListPush(AvmValue)(&list, AvmValueFromChar(option[0]));
                break;
            default:
                AvmArrayListPush(AvmValue)(&list,
                                           AvmValueFromBool(option != NULL));
        }
    }

    return list;
}
