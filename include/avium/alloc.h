/**
 * @file avium/alloc.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Object allocation API.
 * @version 0.2
 * @date 2021-04-11
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

#ifndef AVIUM_ALLOC_H
#define AVIUM_ALLOC_H

#include "avium/types.h"

AVMAPI void* AvmAlloc(size_t size);
AVMAPI void* AvmRealloc(void* memory, size_t size);
AVMAPI void AvmDealloc(void* memory);

AVMAPI object AvmObjectAlloc(size_t size, object data);

#define heapalloc(T, ...) AvmObjectAlloc(sizeof(T), (T[1]){__VA_ARGS__})

#endif  // AVIUM_ALLOC_H
