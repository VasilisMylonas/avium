/**
 * @file avium/array-list.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Generic dynamic array type implementation.
 * @version 0.1
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

#ifndef AVIUM_ARRAY_LIST_H
#define AVIUM_ARRAY_LIST_H

#include "avium/runtime.h"

#define AvmArrayList(T)            AVM_GENERIC(AvmArrayList, T)
#define AvmArrayListNew(T)         AVM_GENERIC(AvmArrayListNew, T)
#define AvmArrayListFrom(T)        AVM_GENERIC(AvmArrayListFrom, T)
#define AvmArrayListPush(T)        AVM_GENERIC(AvmArrayListPush, T)
#define AvmArrayListPop(T)         AVM_GENERIC(AvmArrayListPop, T)
#define AvmArrayListPeek(T)        AVM_GENERIC(AvmArrayListPeek, T)
#define AvmArrayListGetLength(T)   AVM_GENERIC(AvmArrayListGetLength, T)
#define AvmArrayListGetCapacity(T) AVM_GENERIC(AvmArrayListGetCapacity, T)
#define AvmArrayListDestroy(T)     AVM_GENERIC(AvmArrayListDestroy, T)

#define AVM_ARRAY_LIST_TYPE(T)                                                \
    AVM_CLASS(AVM_GENERIC(AvmArrayList, T), object, {                         \
        size_t _length;                                                       \
        size_t _capacity;                                                     \
        T* _items;                                                            \
    });                                                                       \
                                                                              \
    static_assert_s(sizeof(AvmArrayList(T)) == AVM_ARRAY_LIST_SIZE);          \
                                                                              \
    static inline void AvmArrayListDestroy(T)(AvmArrayList(T) * self) {       \
        AvmDealloc(self->_items);                                             \
    }                                                                         \
                                                                              \
    AVM_TYPE(AvmArrayList(T),                                                 \
             {[FUNC_DTOR] = (AvmFunction)AvmArrayListDestroy(T)});            \
                                                                              \
    static inline AvmArrayList(T) AvmArrayListNew(T)(size_t capacity) {       \
        return (AvmArrayList(T)){                                             \
            ._type = AVM_GET_TYPE(AvmArrayList(T)),                           \
            ._length = 0,                                                     \
            ._capacity = capacity,                                            \
            ._items = capacity == 0 ? NULL : AvmAlloc(capacity * sizeof(T)),  \
        };                                                                    \
    }                                                                         \
                                                                              \
    static inline size_t AvmArrayListGetCapacity(T)(AvmArrayList(T) * self) { \
        return self->_capacity;                                               \
    }                                                                         \
                                                                              \
    static inline size_t AvmArrayListGetLength(T)(AvmArrayList(T) * self) {   \
        return self->_length;                                                 \
    }                                                                         \
                                                                              \
    static inline void AvmArrayListPush(T)(AvmArrayList(T) * self, T item) {  \
        if (self->_length >= self->_capacity) {                               \
            self->_capacity *= AVM_ARRAY_LIST_GROWTH_FACTOR;                  \
            self->_items =                                                    \
                AvmRealloc(self->_items, self->_capacity * sizeof(T));        \
        }                                                                     \
                                                                              \
        self->_items[self->_length++] = item;                                 \
    }                                                                         \
                                                                              \
    static inline AvmArrayList(T) AvmArrayListFrom(T)(size_t length, ...) {   \
        va_list args;                                                         \
        va_start(args, length);                                               \
                                                                              \
        AvmArrayList(T) list = AvmArrayListNew(T)(length);                    \
        for (size_t i = 0; i < length; i++) {                                 \
            AvmArrayListPush(T)(&list, va_arg(args, T));                      \
        }                                                                     \
                                                                              \
        va_end(args);                                                         \
        return list;                                                          \
    }                                                                         \
                                                                              \
    static inline T AvmArrayListPop(T)(AvmArrayList(T) * self) {              \
        self->_length--;                                                      \
        return self->_items[self->_length];                                   \
    }                                                                         \
                                                                              \
    static inline T AvmArrayListPeek(T)(AvmArrayList(T) * self) {             \
        return self->_items[self->_length - 1];                               \
    }

#define AVM_ARRAY_LIST_TYPE_PROMOTABLE(T, C)                                  \
    AVM_CLASS(AVM_GENERIC(AvmArrayList, T), object, {                         \
        size_t _length;                                                       \
        size_t _capacity;                                                     \
        T* _items;                                                            \
    });                                                                       \
                                                                              \
    static_assert_s(sizeof(AvmArrayList(T)) == AVM_ARRAY_LIST_SIZE);          \
                                                                              \
    static inline void AvmArrayListDestroy(T)(AvmArrayList(T) * self) {       \
        AvmDealloc((void*)self->_items);                                      \
    }                                                                         \
                                                                              \
    AVM_TYPE(AvmArrayList(T),                                                 \
             {[FUNC_DTOR] = (AvmFunction)AvmArrayListDestroy(T)});            \
                                                                              \
    static inline AvmArrayList(T) AvmArrayListNew(T)(size_t capacity) {       \
        return (AvmArrayList(T)){                                             \
            ._type = AVM_GET_TYPE(AvmArrayList(T)),                           \
            ._length = 0,                                                     \
            ._capacity = capacity,                                            \
            ._items = capacity == 0 ? NULL : AvmAlloc(capacity * sizeof(T)),  \
        };                                                                    \
    }                                                                         \
                                                                              \
    static inline size_t AvmArrayListGetCapacity(T)(AvmArrayList(T) * self) { \
        return self->_capacity;                                               \
    }                                                                         \
                                                                              \
    static inline size_t AvmArrayListGetLength(T)(AvmArrayList(T) * self) {   \
        return self->_length;                                                 \
    }                                                                         \
                                                                              \
    static inline void AvmArrayListPush(T)(AvmArrayList(T) * self, T item) {  \
        if (self->_length >= self->_capacity) {                               \
            self->_capacity *= AVM_ARRAY_LIST_GROWTH_FACTOR;                  \
            self->_items =                                                    \
                AvmRealloc(self->_items, self->_capacity * sizeof(T));        \
        }                                                                     \
                                                                              \
        self->_items[self->_length++] = item;                                 \
    }                                                                         \
                                                                              \
    static inline AvmArrayList(T) AvmArrayListFrom(T)(size_t length, ...) {   \
        va_list args;                                                         \
        va_start(args, length);                                               \
                                                                              \
        AvmArrayList(T) list = AvmArrayListNew(T)(length);                    \
        for (size_t i = 0; i < length; i++) {                                 \
            AvmArrayListPush(T)(&list, (T)va_arg(args, C));                   \
        }                                                                     \
                                                                              \
        va_end(args);                                                         \
        return list;                                                          \
    }                                                                         \
                                                                              \
    static inline T AvmArrayListPop(T)(AvmArrayList(T) * self) {              \
        self->_length--;                                                      \
        return self->_items[self->_length];                                   \
    }                                                                         \
                                                                              \
    static inline T AvmArrayListPeek(T)(AvmArrayList(T) * self) {             \
        return self->_items[self->_length - 1];                               \
    }

AVM_ARRAY_LIST_TYPE_PROMOTABLE(char, int)
AVM_ARRAY_LIST_TYPE_PROMOTABLE(byte, uint)
AVM_ARRAY_LIST_TYPE_PROMOTABLE(short, int)
AVM_ARRAY_LIST_TYPE_PROMOTABLE(ushort, uint)
AVM_ARRAY_LIST_TYPE_PROMOTABLE(bool, uint)
AVM_ARRAY_LIST_TYPE(int)
AVM_ARRAY_LIST_TYPE(uint)
AVM_ARRAY_LIST_TYPE(_long)
AVM_ARRAY_LIST_TYPE(ulong)
AVM_ARRAY_LIST_TYPE(str)
AVM_ARRAY_LIST_TYPE(object)
AVM_ARRAY_LIST_TYPE(size_t)
AVM_ARRAY_LIST_TYPE(AvmString)
AVM_ARRAY_LIST_TYPE(AvmFunction)

#undef AVM_ARRAY_LIST_TYPE_PROMOTABLE

#endif  // AVIUM_ARRAY_LIST_H
