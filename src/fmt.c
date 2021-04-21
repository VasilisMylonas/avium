#include "avium/fmt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef AVM_HAVE_UCHAR_H
#    include <uchar.h>
#endif

#include "avium/string.h"
#include "avium/resources.h"

typedef union {
    float value;

    struct {
        uint mantissaLow : 16;
        uint mantissaHigh : 7;
        uint exponent : 8;
        bool isNegative : 1;
    };
} Float;

AvmString AvmItoa(_long value) {
    if (value == INTMAX_MIN) {
        return AvmStringFrom(LongMinRepr);
    }

    const bool isNegative = value < 0;
    if (isNegative) {
        value = -value;
    }

    AvmString s = AvmStringNew(8);

    size_t i = 0;
    for (; value != 0; i++, value /= 10) {
        _long r = value % 10;
        AvmStringPushChar(&s, '0' + r);
    }

    if (i == 0) {
        AvmStringPushChar(&s, '0');
        i++;
    }

    if (isNegative) {
        AvmStringPushChar(&s, '-');
        i++;
    }

    AvmStringReverse(&s);
    return s;
}

AvmString AvmUtoa(ulong value, AvmNumericBase numericBase) {
    switch (numericBase) {
        case NumericBaseBinary:
        case NumericBaseOctal:
        case NumericBaseDecimal:
        case NumericBaseHex:
            break;
        default:
            AvmPanic(NumericBaseOutOfRangeMsg);
    }

    AvmString s = AvmStringNew(8);

    size_t i = 0;
    for (; value != 0; i++, value /= numericBase) {
        _long r = value % numericBase;

        if (r >= 10) {
            AvmStringPushChar(&s, 'A' + (r - 10));
        } else {
            AvmStringPushChar(&s, '0' + r);
        }
    }

    if (i == 0) {
        AvmStringPushChar(&s, '0');
        i++;
    }

    AvmStringReverse(&s);
    return s;
}

AvmString AvmFtoa2(float value) {
    Float f = {.value = value};

    int8_t exponent = f.exponent - 127;  // Biased exponent.

    if (exponent > 18) {
        // Too big.
        return AvmStringFrom("inf");
    }

    if (exponent < -3) {
        // Too small.
        return AvmStringFrom("0");
    }

    AvmString s = AvmStringNew(8);

    if (f.isNegative) {
        AvmStringPushChar(&s, '-');
    }

    ulong mantissa = f.mantissaLow;
    mantissa += ((ulong)f.mantissaHigh << 16);
    mantissa += 0x00800000;

    AvmString intPart = AvmItoa(mantissa >> (23 - exponent));
    AvmStringPushString(&s, &intPart);
    AvmObjectDestroy(&intPart);

    AvmStringPushChar(&s, '.');

    switch (0x7 & (mantissa >> (20 - exponent))) {
        case 0:
            AvmStringPushStr(&s, "000");
            break;
        case 1:
            AvmStringPushStr(&s, "125");
            break;
        case 2:
            AvmStringPushStr(&s, "250");
            break;
        case 3:
            AvmStringPushStr(&s, "375");
            break;
        case 4:
            AvmStringPushStr(&s, "500");
            break;
        case 5:
            AvmStringPushStr(&s, "625");
            break;
        case 6:
            AvmStringPushStr(&s, "750");
            break;
        case 7:
            AvmStringPushStr(&s, "875");
            break;
    }

    return s;
}

AvmString AvmFtoa(double value) {
    size_t length = snprintf(NULL, 0, "%lf", value);
    AvmString s = AvmStringNew(length);
    char* buffer = AvmStringAsPtr(&s);
    snprintf(buffer, length + 1, "%lf", value);
    AvmStringUnsafeSetLength(&s, length);
    return s;
}

AvmString AvmVSprintf(str format, va_list args) {
    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }

    if (args == NULL) {
        AvmPanic(ArgsNullMsg);
    }

    AvmString s = AvmStringNew(8);

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') {
            AvmStringPushChar(&s, format[i]);
            continue;
        }

        i++;

        switch (format[i]) {
#ifdef AVM_HAVE_UCHAR_H
            case AVM_FMT_UNICODE: {
                AvmString temp =
                    AvmUtoa(va_arg(args, char32_t), NumericBaseHex);
                AvmStringPushStr(&s, AVM_FMT_UNICODE_PREFIX);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
#endif
            case AVM_FMT_INT_DECIMAL: {
                AvmString temp = AvmItoa(va_arg(args, _long));
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_INT_OCTAL: {
                AvmString temp = AvmUtoa(va_arg(args, ulong), NumericBaseOctal);
                AvmStringPushStr(&s, AVM_FMT_OCTAL_PREFIX);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_POINTER:
            case AVM_FMT_INT_HEX: {
                AvmString temp = AvmUtoa(va_arg(args, ulong), NumericBaseHex);
                AvmStringPushStr(&s, AVM_FMT_HEX_PREFIX);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_INT_BINARY: {
                AvmString temp =
                    AvmUtoa(va_arg(args, ulong), NumericBaseBinary);
                AvmStringPushStr(&s, AVM_FMT_BINARY_PREFIX);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_FLOAT: {
                AvmString temp = AvmFtoa(va_arg(args, double));
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_FLOAT_EXP: {
                char buffer[AVM_FLOAT_BUFFER_SIZE] = {0};
                snprintf(buffer, AVM_FLOAT_BUFFER_SIZE, "%le",
                         va_arg(args, double));
                AvmStringPushStr(&s, buffer);
                break;
            }
            case AVM_FMT_FLOAT_AUTO: {
                char buffer[AVM_FLOAT_BUFFER_SIZE] = {0};
                snprintf(buffer, AVM_FLOAT_BUFFER_SIZE, "%lg",
                         va_arg(args, double));
                AvmStringPushStr(&s, buffer);
                break;
            }
            case AVM_FMT_INT_SIZE:
            case AVM_FMT_INT_UNSIGNED: {
                AvmString temp =
                    AvmUtoa(va_arg(args, ulong), NumericBaseDecimal);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_CHAR:
                AvmStringPushChar(&s, (char)va_arg(args, int));
                break;
            case AVM_FMT_STRING:
                AvmStringPushStr(&s, va_arg(args, char*));
                break;
            case AVM_FMT_BOOL:
                AvmStringPushStr(&s, (bool)va_arg(args, uint) ? AVM_FMT_TRUE
                                                              : AVM_FMT_FALSE);
                break;
            case AVM_FMT_TYPE: {
                const AvmType* type = AvmObjectGetType(va_arg(args, object));
                AvmStringPushStr(&s, AvmTypeGetName(type));
                break;
            }
            case AVM_FMT_SIZE: {
                const AvmType* type = AvmObjectGetType(va_arg(args, object));
                AvmString temp =
                    AvmUtoa(AvmTypeGetSize(type), NumericBaseDecimal);
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            case AVM_FMT_VALUE: {
                AvmString temp = AvmObjectToString(va_arg(args, object));
                AvmStringPushString(&s, &temp);
                AvmObjectDestroy(&temp);
                break;
            }
            default:
                AvmStringPushChar(&s, format[i]);
                break;
        }
    }

    return s;
}

void AvmVFprintf(void* handle, str format, va_list args) {
    if (handle == NULL) {
        AvmPanic(HandleNullMsg);
    }

    AvmString s = AvmVSprintf(format, args);
    fwrite(AvmStringAsPtr(&s), sizeof(char), AvmStringGetLength(&s), handle);
    fflush(handle);
    AvmObjectDestroy(&s);
}

void AvmVPrintf(str format, va_list args) { AvmVFprintf(stdout, format, args); }

void AvmVErrorf(str format, va_list args) { AvmVFprintf(stderr, format, args); }

void AvmScanf(str format, ...);
void AvmSscanf(AvmString* string, str format, ...);
AvmString AvmSprintf(str format, ...);
void AvmFprintf(void* handle, str format, ...);
void AvmPrintf(str format, ...);
void AvmErrorf(str format, ...);

static void SkipWord(char* buffer, size_t* index) {
    while (buffer[*index] != ' ' && buffer[*index] != '\0') {
        (*index)++;
    }
}

#define UINT_CASE(base)       \
    char* start = &buffer[j]; \
    SkipWord(buffer, &j);     \
    char* end = &buffer[j];   \
    *((ulong*)va_arg(args, ulong*)) = strtoull(start, &end, base)

void AvmVFscanf(void* handle, str format, va_list args) {
    if (handle == NULL) {
        AvmPanic(HandleNullMsg);
    }

    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }

    // TODO
    AvmString s = AvmStringNew(128);
    char* dummy = fgets(AvmStringAsPtr(&s), AvmStringGetLength(&s), handle);
    (void)dummy;
    AvmVSscanf(&s, format, args);
    AvmObjectDestroy(&s);
}

void AvmVScanf(str format, va_list args) {
    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }

    AvmVFscanf(stdin, format, args);
}

void AvmVSscanf(AvmString* string, str format, va_list args) {
    if (string == NULL) {
        AvmPanic(StringNullMsg);
    }

    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }

    char* buffer = AvmStringAsPtr(string);

    for (size_t i = 0, j = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') {
            continue;
        }

        i++;

        switch (format[i]) {
            case AVM_FMT_CHAR: {
                char* c = va_arg(args, char*);
                *c = buffer[j];
                j++;
                break;
            }
            case AVM_FMT_BOOL: {
                *((bool*)va_arg(args, bool*)) =
                    strncmp(&buffer[j], AVM_FMT_TRUE, 4) == 0;
                SkipWord(buffer, &j);
                break;
            }
            case AVM_FMT_INT_DECIMAL: {
                char* start = &buffer[j];
                SkipWord(buffer, &j);
                char* end = &buffer[j];
                *((_long*)va_arg(args, _long*)) = strtoll(start, &end, 10);
                break;
            }
            case AVM_FMT_INT_BINARY: {
                UINT_CASE(2);
                break;
            }
            case AVM_FMT_INT_OCTAL: {
                UINT_CASE(8);
                break;
            }
            case AVM_FMT_INT_SIZE:
            case AVM_FMT_INT_UNSIGNED: {
                UINT_CASE(10);
                break;
            }
            case AVM_FMT_POINTER:
            case AVM_FMT_INT_HEX: {
                UINT_CASE(16);
                break;
            }
            case AVM_FMT_STRING: {
                char* s = va_arg(args, char*);
                size_t start = j;
                SkipWord(buffer, &j);

                size_t capacity = va_arg(args, size_t);
                size_t length = j - start;

                AvmMemCopy((byte*)&buffer[start], length, (byte*)s, capacity);

                if (length < capacity) {
                    s[length] = '\0';
                } else {
                    s[capacity - 1] = '\0';
                }
            }
            default:
                break;
        }

        j++;
    }
}
