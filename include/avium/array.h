/**
 * @file avium/array.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Generic fixed array type implementation.
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

#ifndef AVIUM_ARRAY_H
#define AVIUM_ARRAY_H

#include "avium/runtime.h"

/**
 * @brief A fixed array of type T and length N.
 *
 * @param T The type of the array elements.
 * @param N The length of the array.
 */
#define AvmArray(T, N) AVM_GENERIC(AvmArray, T##_##N)

/**
 * @brief Creates an AvmArray(T, N) with the specified elements.
 *
 * @param T The type of the array elements.
 * @param N The length of the array.
 * @param ... The elements of the array.
 */
#define AvmArrayFrom(T, N, ...)                                            \
    (AvmArray(T, N)) {                                                     \
        ._type = typeid(AvmArray(T, N)), .length = N, .at = {__VA_ARGS__}, \
    }

/**
 * @brief Declares an AvmArray(T, N) type for type T.
 *
 * @param T The type.
 */
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

#ifndef DOXYGEN
#    define AVM_ARRAY_TYPE_N(T, N)                          \
        AVM_CLASS(AVM_GENERIC(AvmArray, T##_##N), object, { \
            size_t length;                                  \
            T at[N];                                        \
        });                                                 \
                                                            \
        AVM_TYPE(AvmArray(T, N), {[FnEntryDtor] = NULL});

#    define AVM_ARRAY_NATIVE_TYPE_N(T, N)                              \
        AVM_CLASS(AVM_GENERIC(AvmArray, T##_##N), object, {            \
            size_t length;                                             \
            T at[N];                                                   \
        });                                                            \
                                                                       \
        AVMAPI AvmString AvmArrayToString_##T(AvmArray(T, 1) * array); \
                                                                       \
        AVM_TYPE(AvmArray(T, N),                                       \
                 {[FnEntryToString] = (AvmFunction)AvmArrayToString_##T});

#    define AVM_ARRAY_NATIVE_TYPE(T)   \
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
#endif  // DOXYGEN

#ifndef AVM_MSVC
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wunused-variable"
#endif

AVM_ARRAY_NATIVE_TYPE(char)
AVM_ARRAY_NATIVE_TYPE(byte)
AVM_ARRAY_NATIVE_TYPE(short)
AVM_ARRAY_NATIVE_TYPE(ushort)
AVM_ARRAY_NATIVE_TYPE(int)
AVM_ARRAY_NATIVE_TYPE(uint)
AVM_ARRAY_NATIVE_TYPE(_long)
AVM_ARRAY_NATIVE_TYPE(ulong)
AVM_ARRAY_NATIVE_TYPE(str)

#ifndef AVM_MSVC
#    pragma GCC diagnostic pop
#endif

#undef AVM_ARRAY_NATIVE_TYPE_N
#undef AVM_ARRAY_NATIVE_TYPE

#endif  // AVIUM_ARRAY_H
