#include "avium/array.h"

#include "avium/string.h"

#define AVM_ARRAY_TO_STRING_SIGNED(T)                        \
    AvmString AvmArrayToString_##T(AvmArray(T, 1) * array) { \
        AvmString string = AvmStringNew(array->length);      \
        AvmStringPushStr(&string, "[ ");                     \
        for (size_t i = 0; i < array->length; i++) {         \
            AvmString temp = AvmStringFromInt(array->at[i]); \
            AvmStringPushString(&string, &temp);             \
            AvmStringPushStr(&string, ", ");                 \
            AvmObjectDestroy(&temp);                         \
        }                                                    \
        AvmStringPushChar(&string, ']');                     \
        return string;                                       \
    }

#define AVM_ARRAY_TO_STRING_UNSIGNED(T)                              \
    AvmString AvmArrayToString_##T(AvmArray(T, 1) * array) {         \
        AvmString string = AvmStringNew(array->length);              \
        AvmStringPushStr(&string, "[ ");                             \
        for (size_t i = 0; i < array->length; i++) {                 \
            AvmString temp =                                         \
                AvmStringFromUint(array->at[i], NumericBaseDecimal); \
            AvmStringPushString(&string, &temp);                     \
            AvmStringPushStr(&string, ", ");                         \
            AvmObjectDestroy(&temp);                                 \
        }                                                            \
        AvmStringPushChar(&string, ']');                             \
        return string;                                               \
    }

AVM_ARRAY_TO_STRING_SIGNED(short)
AVM_ARRAY_TO_STRING_SIGNED(_long)
AVM_ARRAY_TO_STRING_UNSIGNED(byte)
AVM_ARRAY_TO_STRING_UNSIGNED(ushort)
AVM_ARRAY_TO_STRING_SIGNED(int)
AVM_ARRAY_TO_STRING_UNSIGNED(uint)
AVM_ARRAY_TO_STRING_UNSIGNED(ulong)

AvmString AvmArrayToString_char(AvmArray(char, 1) * array) {
    AvmString string = AvmStringNew(array->length);
    AvmStringPushStr(&string, "[ ");
    for (size_t i = 0; i < array->length; i++) {
        AvmStringPushStr(&string, ", ");
        AvmStringPushChar(&string, array->at[i]);
    }
    AvmStringPushChar(&string, ']');
    return string;
}

AvmString AvmArrayToString_str(AvmArray(str, 1) * array) {
    AvmString string = AvmStringNew(array->length);
    AvmStringPushStr(&string, "[ ");
    for (size_t i = 0; i < array->length; i++) {
        AvmStringPushStr(&string, array->at[i]);
    }
    AvmStringPushChar(&string, ']');
    return string;
}
