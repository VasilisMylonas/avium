/**
 * @file avium/options.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Command line option parsing.
 * @version 0.2
 * @date 2021-04-15
 *
 * @copyright Copyright (c) 2021 Vasilis Mylonas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef AVIUM_OPTIONS_H
#define AVIUM_OPTIONS_H

#include "avium/config.h"

#ifdef AVM_USE_ARGPARSE

#    include "avium/types.h"
#    include "avium/array-list.h"
#    include "avium/value.h"

AVM_ARRAY_LIST_TYPE(AvmValue)

/// Holds information for parsing command-line options.
typedef struct AvmOptionParser AvmOptionParser;

/**
 * @brief Creates an AvmOptionParser
 *
 * @param argc The argc parameter from main.
 * @param argv The argv parameter from main.
 * @return The created AvmOptionParser instance.
 */
AVMAPI AvmOptionParser* AvmOptionParserNew(int argc, str argv[]);

/**
 * @brief
 *
 * @param self
 * @return AVMAPI
 */
AVMAPI void AvmOptionParserAddStandardOptions(AvmOptionParser* self);

/**
 * @brief Adds an option to an AvmOptionParser instance.
 *
 * @param self The AvmOptionParser instance.
 * @param option The name of the option.
 * @param description A description for the option.
 * @param kind The type of value that the option will accept.
 */
AVMAPI void AvmOptionParserAddOption(AvmOptionParser* self, str option,
                                     str description, AvmValueKind kind);

/**
 * @brief Adds an option to an AvmOptionParser instance.
 *
 * @param self The AvmOptionParser instance.
 * @param option The name of the option.
 * @param shortOption A single-letter option alternative.
 * @param description A description for the option.
 * @param kind The type of value that the option will accept.
 */
AVMAPI void AvmOptionParserAddOptionEx(AvmOptionParser* self, str option,
                                       char shortOption, str description,
                                       AvmValueKind kind);

/**
 * @brief Prints a 'usage' dialog according to the options registered with the
 * provided AvmOptionParser.
 *
 * @param self The AvmOptionParser instance.
 * @param description The program description.
 */
AVMAPI void AvmOptionParserShowUsage(AvmOptionParser* self, str description);

#    ifdef DOXYGEN
/**
 * @brief Parses command-line options, returning a list of the string values.
 *
 * @param self The AvmOptionParser instance.
 * @return The list of string values.
 */
AVMAPI AvmArrayList<str> AvmOptionParserParseRaw(AvmOptionParser* self);

/**
 * @brief Parses command-line options, returning a list of values.
 *
 * @param self The AvmOptionParser instance.
 * @return The list of values.
 */
AVMAPI AvmArrayList<AvmValue> AvmOptionParserParse(AvmOptionParser* self);
#    else
/**
 * @brief Parses command-line options, returning a list of the string values.
 *
 * @param self The AvmOptionParser instance.
 * @return The list of string values.
 */
AVMAPI AvmArrayList(str) AvmOptionParserParseRaw(AvmOptionParser* self);

/**
 * @brief Parses command-line options, returning a list of values.
 *
 * @param self The AvmOptionParser instance.
 * @return The list of values.
 */
AVMAPI AvmArrayList(AvmValue) AvmOptionParserParse(AvmOptionParser* self);
#    endif  // DOXYGEN

#endif  // AVM_USE_ARGPARSE

#endif  // AVIUM_OPTIONS_H
