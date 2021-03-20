#ifndef AVIUM_OPTIONS_H
#define AVIUM_OPTIONS_H

#include "avium/prologue.h"

/**
 * @brief Determines whether the specified option is present.
 *
 * @param argc The argc parameter from main.
 * @param argv The argv parameter from main.
 * @param shortOption The short option in the form of -h, but without the -.
 * @param longOption The long option in the form of --help, but without the --.
 *
 * @return true The specified option was present.
 * @return false The specified option was not present.
 */
AVMAPI bool AvmHasOption(int argc, str* argv, char shortOption, str longOption);

/**
 * @brief Retrieves a command line option.
 *
 * @param argc The argc parameter from main.
 * @param argv The argv parameter from main.
 * @param shortOption The short option in the form of -h, but without the -.
 * @param longOption The long option in the form of --help, but without the --.
 *
 * @return AvmSome(str) The command line option value.
 * @return AvmNone The option was not present.
 */
AVMAPI AvmOptional AvmGetOption(int argc, str* argv, char shortOption,
                                str longOption);

#endif  // AVIUM_OPTIONS_H
