/*
 * File: avium/options.h
 * Functions for handling program arguments.
 *
 * About: Author
 * Vasilis Mylonas <vasilismylonas@protonmail.com>
 *
 * Section: License
 *
 * Copyright (C) 2021 Vasilis Mylonas
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

#include "avium/core.h"

typedef struct {
    str longOption;
    char shortOption;
} AvmOption;

#define AvmOption(longOption, shortOption) \
    (AvmOption) { longOption, shortOption }

AVMAPI bool AvmHasOption(int argc, str* argv, AvmOption option);
AVMAPI AvmOptional AvmGetOption(int argc, str* argv, AvmOption option);

#endif  // AVIUM_OPTIONS_H
