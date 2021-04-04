#ifndef AVIUM_ARRAY_H
#define AVIUM_ARRAY_H

#include "avium/types.h"
#include "avium/typeinfo.h"

extern void free(void*);
extern void* malloc(size_t);
extern void* realloc(void*, size_t);

#define AvmArray(T, N)        AVM_GENERIC(AvmArray, T##_##N)
#define AvmArrayFrom(T, N)    AVM_GENERIC(AvmArrayFrom, T##_##N)
#define AvmArrayDestroy(T, N) AVM_GENERIC(AvmArrayDestroy, T##_##N)

#define AVM_ARRAY_TYPE(T)   \
    AVM_ARRAY_TYPE_N(T, 1)  \
    AVM_ARRAY_TYPE_N(T, 2)  \
    AVM_ARRAY_TYPE_N(T, 3)  \
    AVM_ARRAY_TYPE_N(T, 4)  \
    AVM_ARRAY_TYPE_N(T, 5)  \
    AVM_ARRAY_TYPE_N(T, 6)  \
    AVM_ARRAY_TYPE_N(T, 7)  \
    AVM_ARRAY_TYPE_N(T, 8)  \
    AVM_ARRAY_TYPE_N(T, 9)  \
    AVM_ARRAY_TYPE_N(T, 10) \
    AVM_ARRAY_TYPE_N(T, 11) \
    AVM_ARRAY_TYPE_N(T, 12) \
    AVM_ARRAY_TYPE_N(T, 13) \
    AVM_ARRAY_TYPE_N(T, 14) \
    AVM_ARRAY_TYPE_N(T, 15) \
    AVM_ARRAY_TYPE_N(T, 16) \
    AVM_ARRAY_TYPE_N(T, 17) \
    AVM_ARRAY_TYPE_N(T, 18) \
    AVM_ARRAY_TYPE_N(T, 19) \
    AVM_ARRAY_TYPE_N(T, 20) \
    AVM_ARRAY_TYPE_N(T, 21) \
    AVM_ARRAY_TYPE_N(T, 22) \
    AVM_ARRAY_TYPE_N(T, 23) \
    AVM_ARRAY_TYPE_N(T, 24) \
    AVM_ARRAY_TYPE_N(T, 25) \
    AVM_ARRAY_TYPE_N(T, 26) \
    AVM_ARRAY_TYPE_N(T, 27) \
    AVM_ARRAY_TYPE_N(T, 28) \
    AVM_ARRAY_TYPE_N(T, 29) \
    AVM_ARRAY_TYPE_N(T, 30) \
    AVM_ARRAY_TYPE_N(T, 31) \
    AVM_ARRAY_TYPE_N(T, 32) \
    AVM_ARRAY_TYPE_N(T, 33) \
    AVM_ARRAY_TYPE_N(T, 34) \
    AVM_ARRAY_TYPE_N(T, 35) \
    AVM_ARRAY_TYPE_N(T, 36) \
    AVM_ARRAY_TYPE_N(T, 37) \
    AVM_ARRAY_TYPE_N(T, 38) \
    AVM_ARRAY_TYPE_N(T, 39) \
    AVM_ARRAY_TYPE_N(T, 40) \
    AVM_ARRAY_TYPE_N(T, 41) \
    AVM_ARRAY_TYPE_N(T, 42) \
    AVM_ARRAY_TYPE_N(T, 43) \
    AVM_ARRAY_TYPE_N(T, 44) \
    AVM_ARRAY_TYPE_N(T, 45) \
    AVM_ARRAY_TYPE_N(T, 46) \
    AVM_ARRAY_TYPE_N(T, 47) \
    AVM_ARRAY_TYPE_N(T, 48) \
    AVM_ARRAY_TYPE_N(T, 49) \
    AVM_ARRAY_TYPE_N(T, 50) \
    AVM_ARRAY_TYPE_N(T, 51) \
    AVM_ARRAY_TYPE_N(T, 52) \
    AVM_ARRAY_TYPE_N(T, 53) \
    AVM_ARRAY_TYPE_N(T, 54) \
    AVM_ARRAY_TYPE_N(T, 55) \
    AVM_ARRAY_TYPE_N(T, 56) \
    AVM_ARRAY_TYPE_N(T, 57) \
    AVM_ARRAY_TYPE_N(T, 58) \
    AVM_ARRAY_TYPE_N(T, 59) \
    AVM_ARRAY_TYPE_N(T, 60) \
    AVM_ARRAY_TYPE_N(T, 61) \
    AVM_ARRAY_TYPE_N(T, 62) \
    AVM_ARRAY_TYPE_N(T, 63) \
    AVM_ARRAY_TYPE_N(T, 64)

#define AVM_ARRAY_TYPE_N(T, N)                                        \
    AVM_CLASS(AVM_GENERIC(AvmArray, T##_##N), object, {               \
        size_t length;                                                \
        T at[N];                                                      \
    });                                                               \
                                                                      \
    static inline void AvmArrayDestroy(T, N)(AvmArray(T, N) * self) { \
        free(self->at);                                               \
    }                                                                 \
                                                                      \
    AVM_TYPE(AvmArray(T, N),                                          \
             {[FUNC_DTOR] = (AvmFunction)AvmArrayDestroy(T, N)});     \
                                                                      \
    static inline AvmArray(T, N) AvmArrayFrom(T, N)(T first, ...) {   \
        AvmArray(T, N) array = {                                      \
            ._type = AVM_GET_TYPE(AvmArray(T, N)),                    \
            .length = N,                                              \
        };                                                            \
                                                                      \
        va_list args;                                                 \
        va_start(args, first);                                        \
        array.at[0] = first;                                          \
        for (size_t i = 1; i < N; i++) {                              \
            array.at[i] = va_arg(args, T);                            \
        }                                                             \
        va_end(args);                                                 \
                                                                      \
        return array;                                                 \
    }

#define AVM_ARRAY_NATIVE_TYPE_N(T, N)                                 \
    AVM_CLASS(AVM_GENERIC(AvmArray, T##_##N), object, {               \
        size_t length;                                                \
        T at[N];                                                      \
    });                                                               \
                                                                      \
    static inline void AvmArrayDestroy(T, N)(AvmArray(T, N) * self) { \
        free(self->at);                                               \
    }                                                                 \
                                                                      \
    AVMAPI AvmString AvmArrayToString_##T(AvmArray(T, 1) * array);    \
                                                                      \
    AVM_TYPE(AvmArray(T, N),                                          \
             {[FUNC_DTOR] = (AvmFunction)AvmArrayDestroy(T, N),       \
              [FUNC_TO_STRING] = (AvmFunction)AvmArrayToString_##T}); \
                                                                      \
    static inline AvmArray(T, N) AvmArrayFrom(T, N)(T first, ...) {   \
        AvmArray(T, N) array = {                                      \
            ._type = AVM_GET_TYPE(AvmArray(T, N)),                    \
            .length = N,                                              \
        };                                                            \
                                                                      \
        va_list args;                                                 \
        va_start(args, first);                                        \
        array.at[0] = first;                                          \
        for (size_t i = 1; i < N; i++) {                              \
            array.at[i] = va_arg(args, T);                            \
        }                                                             \
        va_end(args);                                                 \
                                                                      \
        return array;                                                 \
    }

#define AVM_ARRAY_NATIVE_TYPE(T)   \
    AVM_ARRAY_NATIVE_TYPE_N(T, 1)  \
    AVM_ARRAY_NATIVE_TYPE_N(T, 2)  \
    AVM_ARRAY_NATIVE_TYPE_N(T, 3)  \
    AVM_ARRAY_NATIVE_TYPE_N(T, 4)  \
    AVM_ARRAY_NATIVE_TYPE_N(T, 5)  \
    AVM_ARRAY_NATIVE_TYPE_N(T, 6)  \
    AVM_ARRAY_NATIVE_TYPE_N(T, 7)  \
    AVM_ARRAY_NATIVE_TYPE_N(T, 8)  \
    AVM_ARRAY_NATIVE_TYPE_N(T, 9)  \
    AVM_ARRAY_NATIVE_TYPE_N(T, 10) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 11) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 12) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 13) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 14) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 15) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 16) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 17) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 18) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 19) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 20) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 21) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 22) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 23) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 24) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 25) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 26) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 27) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 28) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 29) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 30) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 31) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 32) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 33) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 34) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 35) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 36) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 37) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 38) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 39) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 40) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 41) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 42) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 43) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 44) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 45) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 46) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 47) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 48) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 49) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 50) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 51) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 52) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 53) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 54) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 55) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 56) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 57) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 58) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 59) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 60) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 61) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 62) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 63) \
    AVM_ARRAY_NATIVE_TYPE_N(T, 64)

AVM_ARRAY_NATIVE_TYPE(char)
AVM_ARRAY_NATIVE_TYPE(byte)
AVM_ARRAY_NATIVE_TYPE(short)
AVM_ARRAY_NATIVE_TYPE(ushort)
AVM_ARRAY_NATIVE_TYPE(int)
AVM_ARRAY_NATIVE_TYPE(uint)
AVM_ARRAY_NATIVE_TYPE(_long)
AVM_ARRAY_NATIVE_TYPE(ulong)
AVM_ARRAY_NATIVE_TYPE(str)

#undef AVM_ARRAY_NATIVE_TYPE_N
#undef AVM_ARRAY_NATIVE_TYPE

#endif  // AVIUM_ARRAY_H
