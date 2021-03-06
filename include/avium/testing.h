/*
 * File: avium/testing.h
 * TAP format testing implementation.
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

#include "avium/core.h"

AVMAPI void Ok(bool expression, const char8_t* description);
AVMAPI void Plan(size_t count);
AVMAPI void Skip(size_t count);
AVMAPI void Note(const char8_t* message);
AVMAPI void BailOut(const char8_t* reason);
AVMAPI void Pass(const char8_t* description);
AVMAPI void Fail(const char8_t* description);
AVMAPI void Todo(const char8_t* description);
AVMAPI void AvmTestSetOutput(void* stream);
