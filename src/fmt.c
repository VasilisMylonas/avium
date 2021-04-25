#include "avium/fmt.h"

#include "avium/string.h"
#include "avium/resources.h"
#include "avium/runtime.h"

#include <stdint.h>
#include <stdio.h>

typedef union {
    float value;

    struct {
        uint mantissaLow : 16;
        uint mantissaHigh : 7;
        uint exponent : 8;
        bool isNegative : 1;
    };
} Float;

AvmString AvmStringFromInt(_long value) {
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

AvmString AvmStringFromUint(ulong value, AvmNumericBase numericBase) {
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

AvmString AvmStringFromFloat2(float value) {
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

    AvmString intPart = AvmStringFromInt(mantissa >> (23 - exponent));
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

AvmString AvmStringFromFloat(double value) {
    size_t length = snprintf(NULL, 0, "%lf", value);
    AvmString s = AvmStringNew(length);
    char* buffer = AvmStringAsPtr(&s);
    snprintf(buffer, length + 1, "%lf", value);
    AvmStringUnsafeSetLength(&s, length);
    return s;
}

#define AVM_FORWARD(arg, call) \
    va_list args;              \
    va_start(args, arg);       \
    call(arg, args);           \
    va_end(args);

void AvmVScanf(str format, va_list args) {
    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }

    vscanf(format, args);
}

void AvmVPrintf(str format, va_list args) {
    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }

    vfprintf(stdout, format, args);
}

void AvmVErrorf(str format, va_list args) {
    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }

    vfprintf(stderr, format, args);
}

void AvmScanf(str format, ...) {
    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }
    AVM_FORWARD(format, AvmVScanf);
}

void AvmPrintf(str format, ...) {
    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }
    AVM_FORWARD(format, AvmVPrintf);
}

void AvmErrorf(str format, ...) {
    if (format == NULL) {
        AvmPanic(FormatNullMsg);
    }
    AVM_FORWARD(format, AvmVErrorf);
}
