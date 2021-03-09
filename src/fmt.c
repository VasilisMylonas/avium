#include "avium/fmt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>

#include "avium/string.h"

#define AVM_FLOAT_BUFFER_SIZE 128

static const char8_t* const FormatNullMsg = "Parameter `format` was `NULL`.";
static const char8_t* const ArgsNullMsg = "Parameter `args` was `NULL`.";
static const char8_t* const HandleNullMsg = "Parameter `handle` was `NULL`.";

typedef union {
    float value;

    struct {
        uint32_t mantissaLow : 16;
        uint32_t mantissaHigh : 7;
        uint32_t exponent : 8;
        bool isNegative : 1;
    };
} Float;

AvmString AvmItoa(intmax_t value) {
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
        int64_t r = value % 10;
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

AvmString AvmUtoa(uintmax_t value, uint8_t base) {
    const bool isNegative = value < 0;
    if (isNegative) {
        value = -value;
    }

    AvmString s = AvmString(8);

    size_t i = 0;
    for (; value != 0; i++, value /= base) {
        int64_t r = value % base;

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

    if (isNegative) {
        s = AvmStringAppendChar(s, '-');
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

    uint64_t mantissa = f.mantissaLow;
    mantissa += ((uint64_t)f.mantissaHigh << 16);
    mantissa += 0x00800000;

    AvmString intPart = AvmItoa(mantissa >> (23 - exponent));
    s = AvmStringConcat(s, intPart);
    AvmDestroy(intPart);

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
    char8_t* buffer = AvmStringAsPtr(s);
    snprintf(buffer, length + 1, "%lf", value);
    return s;
}

AvmString AvmVSnprintf(const char8_t* format, size_t length, va_list args) {
    if (format == NULL) {
        panic(FormatNullMsg);
    }

    if (args == NULL) {
        panic(ArgsNullMsg);
    }

    AvmString s = AvmString(8);

    for (size_t i = 0; i < length; i++) {
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
                temp = AvmItoa(va_arg(args, int64_t));
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_INT_OCTAL:
                temp = AvmUtoa(va_arg(args, uint64_t), 8);
                s = AvmStringAppend(s, "0o");
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_POINTER:
            case AVM_FMT_INT_HEX:
                temp = AvmUtoa(va_arg(args, uint64_t), 16);
                s = AvmStringAppend(s, "0x");
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_INT_BINARY:
                temp = AvmUtoa(va_arg(args, uint64_t), 2);
                s = AvmStringAppend(s, "0b");
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_FLOAT:
                temp = AvmFtoa(va_arg(args, double));
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_FLOAT_EXP: {
                char8_t buffer[AVM_FLOAT_BUFFER_SIZE] = {0};
                snprintf(buffer, AVM_FLOAT_BUFFER_SIZE, "%le",
                         va_arg(args, double));
                temp = AvmStringAppend(temp, buffer);
                break;
            }
            case AVM_FMT_FLOAT_AUTO: {
                char8_t buffer[AVM_FLOAT_BUFFER_SIZE] = {0};
                snprintf(buffer, AVM_FLOAT_BUFFER_SIZE, "%lg",
                         va_arg(args, double));
                temp = AvmStringAppend(temp, buffer);
                break;
            }
            case AVM_FMT_INT_SIZE:
            case AVM_FMT_INT_UNSIGNED:
                temp = AvmUtoa(va_arg(args, uint64_t), 10);
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_CHAR:
                s = AvmStringAppendChar(s, (char8_t)va_arg(args, int));
                break;
            case AVM_FMT_STRING:
                s = AvmStringAppend(s, va_arg(args, char8_t*));
                break;
            case AVM_FMT_BOOL:
                s = AvmStringAppend(
                    s, (bool)va_arg(args, uint32_t) ? "true" : "false");
                break;
            case AVM_FMT_TYPE:
                s = AvmStringAppend(s, AvmObjectName(va_arg(args, object_t)));
                break;
            case AVM_FMT_SIZE:
                temp = AvmUtoa(AvmObjectSize(va_arg(args, object_t)), 10);
                s = AvmStringConcat(s, temp);
                break;
            case AVM_FMT_VALUE:
                temp = AvmToString(va_arg(args, object_t));
                s = AvmStringAppend(s, AvmStringAsPtr(temp));
                break;
            default:
                s = AvmStringAppendChar(s, format[i]);
                break;
        }

        if (temp != NULL) {
            AvmDestroy(temp);
        }
    }

    return s;
}

AvmString AvmVSprintf(const char8_t* format, va_list args) {
    if (format == NULL) {
        panic(FormatNullMsg);
    }

    if (args == NULL) {
        panic(ArgsNullMsg);
    }

    return AvmVSnprintf(format, strlen(format), args);
}

void AvmVFprintf(void* handle, const char8_t* format, va_list args) {
    if (handle == NULL) {
        panic(HandleNullMsg);
    }

    AvmString s = AvmVSprintf(format, args);
    fputs(AvmStringAsPtr(s), handle);
    AvmDestroy(s);
}

void AvmVPrintf(const char8_t* format, va_list args) {
    AvmVFprintf(stdout, format, args);
}

void AvmVErrorf(const char8_t* format, va_list args) {
    AvmVFprintf(stderr, format, args);
}

AVMAPI AvmString AvmSnprintf(const char8_t* format, size_t length, ...);
AVMAPI AvmString AvmSprintf(const char8_t* format, ...);
AVMAPI void AvmFprintf(void* handle, const char8_t* format, ...);
AVMAPI void AvmPrintf(const char8_t* format, ...);
AVMAPI void AvmErrorf(const char8_t* format, ...);
