#ifndef AVIUM_ARRAY_LIST_H
#define AVIUM_ARRAY_LIST_H

#include "avium/collection.h"

#define AvmArrayList(T)            AVM_GENERIC(AvmArrayList, T)
#define AvmArrayListNew(T)         AVM_GENERIC(AvmArrayListNew, T)
#define AvmArrayListFrom(T)        AVM_GENERIC(AvmArrayListFrom, T)
#define AvmArrayListPush(T)        AVM_GENERIC(AvmArrayListPush, T)
#define AvmArrayListPop(T)         AVM_GENERIC(AvmArrayListPop, T)
#define AvmArrayListPeek(T)        AVM_GENERIC(AvmArrayListPeek, T)
#define AvmArrayListGetLength(T)   AVM_GENERIC(AvmArrayListGetLength, T)
#define AvmArrayListGetCapacity(T) AVM_GENERIC(AvmArrayListGetCapacity, T)

#define AVM_ARRAY_LIST_TYPE(T)                                                 \
    AVM_CLASS(AVM_GENERIC(AvmArrayList, T), object, {                          \
        size_t _length;                                                        \
        size_t _capacity;                                                      \
        T* _items;                                                             \
    });                                                                        \
                                                                               \
    static inline AvmArrayList(T) AvmArrayListNew(T)(size_t capacity) {        \
        return (AvmArrayList(T)){                                              \
            ._length = 0,                                                      \
            ._capacity = capacity,                                             \
            ._items = capacity == 0 ? NULL : malloc(capacity * sizeof(T)),     \
        };                                                                     \
    }                                                                          \
                                                                               \
    static inline size_t AvmArrayListGetCapacity(T)(AvmArrayList(T) * self) {  \
        return self->_capacity;                                                \
    }                                                                          \
                                                                               \
    static inline size_t AvmArrayListGetLength(T)(AvmArrayList(T) * self) {    \
        return self->_length;                                                  \
    }                                                                          \
                                                                               \
    static inline void AvmArrayListPush(T)(AvmArrayList(T) * self, T item) {   \
        if (self->_length >= self->_capacity) {                                \
            self->_capacity *= AVM_ARRAY_LIST_GROWTH_FACTOR;                   \
            self->_items = realloc(self->_items, self->_capacity * sizeof(T)); \
        }                                                                      \
                                                                               \
        self->_items[self->_length++] = item;                                  \
    }                                                                          \
                                                                               \
    static inline AvmArrayList(T) AvmArrayListFrom(T)(size_t length, ...) {    \
        va_list args;                                                          \
        va_start(args, length);                                                \
                                                                               \
        AvmArrayList(T) list = AvmArrayListNew(T)(length);                     \
        for (size_t i = 0; i < length; i++) {                                  \
            AvmArrayListPush(T)(&list, va_arg(args, T));                       \
        }                                                                      \
                                                                               \
        va_end(args);                                                          \
        return list;                                                           \
    }                                                                          \
                                                                               \
    static inline T AvmArrayListPop(T)(AvmArrayList(T) * self) {               \
        self->_length--;                                                       \
        return self->_items[self->_length];                                    \
    }                                                                          \
                                                                               \
    static inline T AvmArrayListPeek(T)(AvmArrayList(T) * self) {              \
        return self->_items[self->_length - 1];                                \
    }

#endif  // AVIUM_ARRAY_LIST_H
