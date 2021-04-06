/**
 * @file avium/options.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Command line option parsing.
 * @version 0.2
 * @date 2021-04-06
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

#include "avium/types.h"
#include "avium/array-list.h"
#include "avium/value.h"

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
