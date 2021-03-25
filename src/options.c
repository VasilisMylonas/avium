#include "avium/options.h"
#include "avium/fmt.h"
#include "avium/string.h"

#include <string.h>

AVM_TYPE(AvmOption, {0});

AvmOption AvmOptionFrom(str option, str description, AvmOptionKind kind) {
    return AvmOptionFromEx(option, option[0], description, kind);
}

AvmOption AvmOptionFromEx(str option, char shortOption, str description,
                          AvmOptionKind kind) {
    return (AvmOption){
        ._type = AVM_GET_TYPE(AvmOption),
        .longOption = option,
        .shortOption = shortOption,
        .description = description,
        .kind = kind,
    };
}

static void AvmOptionParserDestroy(AvmOptionParser* self) {
    AvmObjectDestroy(&self->options);
}

AVM_TYPE(AvmOptionParser, {[FUNC_DTOR] = (AvmFunction)AvmOptionParserDestroy});

void AvmOptionParserInit(AvmOptionParser* self, int argc, str argv[]) {
    self->_type = AVM_GET_TYPE(AvmOptionParser);
    self->argc = argc;
    self->argv = argv;
    self->options = AvmArrayListNew(AvmOption)(4);
}

void AvmOptionParserAddOption(AvmOptionParser* self, AvmOption option) {
    AvmArrayListPush(AvmOption)(&self->options, option);
}

void AvmOptionParserAddStandardOptions(AvmOptionParser* self) {
    AvmOption options[] = {
        AvmOptionFrom(
            "verbose",
            "Show extended information about what the program is doing.",
            OK_FLAG),
        AvmOptionFrom("help", "Show the help prompt.", OK_FLAG),
        AvmOptionFromEx("version", 'V', "Show version information.", OK_FLAG),
    };

    AvmOptionParserAddOption(self, options[0]);
    AvmOptionParserAddOption(self, options[1]);
    AvmOptionParserAddOption(self, options[2]);
}

void AvmOptionParserShowUsage(AvmOptionParser* self, str description) {
    str s = strrchr(self->argv[0], '/') + 1;
    AvmPrintf("usage: %s ", s);

    const size_t length = AvmArrayListGetLength(AvmOption)(&self->options);

    for (size_t i = 0; i < length; i++) {
        AvmPrintf("[-%c", self->options._items[i].shortOption);

        if (self->options._items[i].kind != OK_FLAG) {
            AvmPrintf(" %s", self->options._items[i].longOption);
        }

        AvmPrintf("] ");
    }

    AvmPrintf("\n\n%s\n\nOptional Arguments:\n", description);

    for (size_t i = 0; i < length; i++) {
        if (self->options._items[i].kind == OK_FLAG) {
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
                          self->options._items[i].shortOption, s,
                          self->options._items[i].longOption, s,
                          self->options._items[i].description);
                AvmObjectDestroy(&s);
            } else {
                AvmString s = AvmStringFrom(self->options._items[i].longOption);
                AvmStringToUpper(&s);
                AvmPrintf("  --%s=%v - %s\n",
                          self->options._items[i].longOption, s,
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
    if (option->kind != OK_FLAG) {
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
    if (option->kind != OK_FLAG) {
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

AvmArrayList(str) AvmOptionParserParse(AvmOptionParser* self) {
    AvmArrayList(str) list =
        AvmArrayListNew(str)(AvmArrayListGetLength(AvmOption)(&self->options));

    for (size_t i = 0; i < AvmArrayListGetLength(AvmOption)(&self->options);
         i++) {
        AvmArrayListPush(str)(&list, AvmGetOption(self->argc, self->argv,
                                                  &self->options._items[i]));
    }

    return list;
}
