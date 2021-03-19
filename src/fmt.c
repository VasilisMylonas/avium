#include "avium/fmt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <uchar.h>

#include "avium/object.h"
#include "avium/internal.h"

#define AVM_FLOAT_BUFFER_SIZE 128

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
        return AvmStringFrom("-9223372036854775808");
    }

    const bool isNegative = value < 0;
    if (isNegative) {
        value = -value;
    }

    AvmString s = AvmString(8);

    size_t i = 0;
    for (; value != 0; i++, value /= 10) {
        _long r = value % 10;
        s = AvmStringAppendChar(s, '0' + r);
    }

    if (i == 0) {
        s = AvmStringAppendChar(s, '0');
        i++;
    }

    if (isNegative) {
        s = AvmStringAppendChar(s, '-');
        i++;
    }

    AvmStringReverse(s);
    return s;
}

AvmString AvmUtoa(ulong value, NumericBase base) {
    switch (base) {
        case NB_BINARY:
        case NB_OCTAL:
        case NB_DECIMAL:
        case NB_HEX:
            break;
        default:
            panic(BaseOutOfRangeMsg);
    }

    AvmString s = AvmString(8);

    size_t i = 0;
    for (; value != 0; i++, value /= base) {
        _long r = value % base;

        if (r >= 10) {
            s = AvmStringAppendChar(s, 'A' + (r - 10));
        } else {
            s = AvmStringAppendChar(s, '0' + r);
        }
    }

    if (i == 0) {
        s = AvmStringAppendChar(s, '0');
        i++;
    }

    AvmStringReverse(s);
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

    AvmString s = AvmString(8);

    if (f.isNegative) {
        s = AvmStringAppendChar(s, '-');
    }

    ulong mantissa = f.mantissaLow;
    mantissa += ((ulong)f.mantissaHigh << 16);
    mantissa += 0x00800000;

    AvmString intPart = AvmItoa(mantissa >> (23 - exponent));
    s = AvmStringConcat(s, intPart);
    AvmObjectDestroy(intPart);

    s = AvmStringAppendChar(s, '.');

    switch (0x7 & (mantissa >> (20 - exponent))) {
        case 0:
            s = AvmStringAppend(s, "000");
            break;
        case 1:
            s = AvmStringAppend(s, "125");
            break;
        case 2:
            s = AvmStringAppend(s, "250");
            break;
        case 3:
            s = AvmStringAppend(s, "375");
            break;
        case 4:
            s = AvmStringAppend(s, "500");
            break;
        case 5:
            s = AvmStringAppend(s, "625");
            break;
        case 6:
            s = AvmStringAppend(s, "750");
            break;
        case 7:
            s = AvmStringAppend(s, "875");
            break;
    }

    return s;
}

AvmString AvmFtoa(double value) {
    size_t length = snprintf(NULL, 0, "%lf", value);
    AvmString s = AvmString(length);
    char* buffer = AvmStringAsPtr(s);
    snprintf(buffer, length + 1, "%lf", value);
    AvmStringUnsafeSetLength(s, length);
    return s;
}

AvmString AvmVSprintf(str format, va_list args) {
    if (format == NULL) {
        panic(FormatNullMsg);
    }

    if (args == NULL) {
        panic(ArgsNullMsg);
    }

    AvmString s = AvmString(8);

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') {
            s = AvmStringAppendChar(s, format[i]);
            continue;
        }

        i++;

        AvmString temp = NULL;

        switch (format[i]) {
            case AVM_FMT_UNICODE:
                temp = AvmUtoa(va_arg(args, char32_t), 16);
                s = AvmStringAppend(s, "U+");
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_INT_DECIMAL:
                temp = AvmItoa(va_arg(args, _long));
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_INT_OCTAL:
                temp = AvmUtoa(va_arg(args, ulong), 8);
                s = AvmStringAppend(s, "0o");
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_POINTER:
            case AVM_FMT_INT_HEX:
                temp = AvmUtoa(va_arg(args, ulong), 16);
                s = AvmStringAppend(s, "0x");
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_INT_BINARY:
                temp = AvmUtoa(va_arg(args, ulong), 2);
                s = AvmStringAppend(s, "0b");
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_FLOAT:
                temp = AvmFtoa(va_arg(args, double));
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_FLOAT_EXP: {
                char buffer[AVM_FLOAT_BUFFER_SIZE] = {0};
                snprintf(buffer, AVM_FLOAT_BUFFER_SIZE, "%le",
                         va_arg(args, double));
                temp = AvmStringAppend(temp, buffer);
                break;
            }
            case AVM_FMT_FLOAT_AUTO: {
                char buffer[AVM_FLOAT_BUFFER_SIZE] = {0};
                snprintf(buffer, AVM_FLOAT_BUFFER_SIZE, "%lg",
                         va_arg(args, double));
                temp = AvmStringAppend(temp, buffer);
                break;
            }
            case AVM_FMT_INT_SIZE:
            case AVM_FMT_INT_UNSIGNED:
                temp = AvmUtoa(va_arg(args, ulong), 10);
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_CHAR:
                s = AvmStringAppendChar(s, (char)va_arg(args, int));
                break;
            case AVM_FMT_STRING:
                s = AvmStringAppend(s, va_arg(args, char*));
                break;
            case AVM_FMT_BOOL:
                s = AvmStringAppend(
                    s, (bool)va_arg(args, uint) ? "true" : "false");
                break;
            case AVM_FMT_TYPE: {
                AvmType type = AvmObjectType(va_arg(args, object));
                s = AvmStringAppend(s, AvmTypeGetName(type));
                break;
            }
            case AVM_FMT_SIZE: {
                AvmType type = AvmObjectType(va_arg(args, object));
                temp = AvmUtoa(AvmTypeGetSize(type), 10);
                s = AvmStringConcat(s, temp);
                break;
            }
            case AVM_FMT_VALUE:
                temp = AvmObjectToString(va_arg(args, object));
                s = AvmStringAppend(s, AvmStringAsPtr(temp));
                break;
            default:
                s = AvmStringAppendChar(s, format[i]);
                break;
        }

        if (temp != NULL) {
            AvmObjectDestroy(temp);
        }
    }

    return s;
}

void AvmVFprintf(void* handle, str format, va_list args) {
    if (handle == NULL) {
        panic(HandleNullMsg);
    }

    AvmString s = AvmVSprintf(format, args);
    fputs(AvmStringAsPtr(s), handle);
    AvmObjectDestroy(s);
}

void AvmVPrintf(str format, va_list args) { AvmVFprintf(stdout, format, args); }

void AvmVErrorf(str format, va_list args) { AvmVFprintf(stderr, format, args); }

void AvmScanf(str format, ...);
void AvmSscanf(AvmString string, str format, ...);
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
        panic(HandleNullMsg);
    }

    if (format == NULL) {
        panic(FormatNullMsg);
    }

    defer(AvmString, s, AvmString(128)) {
        char* dummy = fgets(AvmStringAsPtr(s), 128, handle);
        (void)dummy;
        AvmVSscanf(s, format, args);
    }
}

void AvmVScanf(str format, va_list args) {
    if (format == NULL) {
        panic(FormatNullMsg);
    }

    AvmVFscanf(stdin, format, args);
}

void AvmVSscanf(AvmString string, str format, va_list args) {
    if (string == NULL) {
        panic(StringNullMsg);
    }

    if (format == NULL) {
        panic(FormatNullMsg);
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
                    strncmp(&buffer[j], "true", 4) == 0;
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
