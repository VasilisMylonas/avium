#include "avium/options.h"

#include <string.h>

static inline bool IsLongOption(const char* arg) {
    return arg[0] == '-' && arg[1] == '-' && arg[2] != '-';
}

static inline bool IsShortOption(const char* arg) {
    return arg[0] == '-' && arg[1] != '-';
}

static bool IsOption(const char* arg, char shortOption,
                     const char* longOption) {
    if (shortOption != 0 && IsShortOption(arg)) {
        return arg[1] == shortOption;
    }

    if (longOption != NULL && IsLongOption(arg)) {
        return strncmp(arg + 2, longOption, strlen(longOption)) == 0;
    }

    return false;
}

static bool OptionHasArgument(const char* arg, size_t length) {
    size_t offset = IsLongOption(arg) ? 2 : 1;

    return arg[offset + length] == '=' && arg[offset + length + 1] != '\0';
}

static const char* OptionGetArgument(const char* arg, size_t length) {
    size_t offset = IsLongOption(arg) ? 2 : 1;
    return arg + offset + length + 1;
}

bool AvmHasOption(int argc, const char** argv, AvmOption option) {
    for (int i = 0; i < argc; i++) {
        if (IsOption(argv[i], option.shortOption, option.longOption)) {
            return true;
        }
    }

    return false;
}

AvmOptional AvmGetOption(int argc, const char** argv, AvmOption option) {
    const size_t length = strlen(option.longOption);

    for (int i = 1; i < argc; i++) {
        if (IsOption(argv[i], option.shortOption, option.longOption)) {
            if (OptionHasArgument(argv[i], length)) {
                return AvmSome((object)OptionGetArgument(argv[i], length));
            }
        }
    }

    return AvmNone();
}
