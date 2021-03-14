/*
 * File: avium/internal.h
 * Header file for internal use.
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

static const char* const SelfNullMsg = "Parameter `self` was `NULL`.";
static const char* const ContentsNullMsg = "Parameter `contents` was `NULL`.";

static const char* const SourceNullMsg = "Parameter `source` was `NULL`.";
static const char* const DestNullMsg = "Parameter `destination` was `NULL`.";
static const char* const FormatNullMsg = "Parameter `format` was `NULL`.";
static const char* const ArgsNullMsg = "Parameter `args` was `NULL`.";
static const char* const HandleNullMsg = "Parameter `handle` was `NULL`.";
static const char* const BaseOutOfRangeMsg =
    "Parameter `base` was out of range`.";
static const char* const StringNullMsg = "Parameter `string` was `NULL`.";
static const char* const OtherNullMsg = "Parameter `other` was `NULL`.";

struct _AvmType {
    const AvmFunction* vptr;
    const char* name;
    size_t size;
};

#define TYPE(T, ...)                                         \
    static const struct _AvmType T##Type = {                 \
        .vptr = (AvmFunction[AVM_VTABLE_SIZE]){__VA_ARGS__}, \
        .name = #T,                                          \
        .size = sizeof(struct _##T),                         \
    }

#define GET_TYPE(T) (AvmType) & T##Type

enum {
    AVM_VTABLE_SIZE = 32,
    FUNC_DTOR = 0,
    FUNC_GET_LENGTH,
    FUNC_GET_CAPACITY,
    FUNC_TO_STRING,
    FUNC_CLONE,
    FUNC_EQ,
    FUNC_READ,
    FUNC_WRITE,
    FUNC_READ_STRING,
    FUNC_WRITE_STRING,
};
