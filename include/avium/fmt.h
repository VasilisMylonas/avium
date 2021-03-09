#ifndef AVIUM_FMT_H
#define AVIUM_FMT_H

#include "avium/core.h"

#define AVM_FMT_INT_DECIMAL  'i'
#define AVM_FMT_INT_HEX      'x'
#define AVM_FMT_INT_BINARY   'b'
#define AVM_FMT_INT_OCTAL    'o'
#define AVM_FMT_INT_UNSIGNED 'u'
#define AVM_FMT_INT_SIZE     'z'
#define AVM_FMT_CHAR         'c'
#define AVM_FMT_UNICODE      'U'
#define AVM_FMT_FLOAT        'f'
#define AVM_FMT_FLOAT_EXP    'e'
#define AVM_FMT_FLOAT_AUTO   'g'
#define AVM_FMT_STRING       's'
#define AVM_FMT_BOOL         't'
#define AVM_FMT_POINTER      'p'
#define AVM_FMT_VALUE        'v'
#define AVM_FMT_TYPE         'T'
#define AVM_FMT_SIZE         'Z'

AVMAPI AvmString AvmItoa(intmax_t value);
AVMAPI AvmString AvmUtoa(uintmax_t value, uint8_t base);

AVMAPI AvmString AvmFtoa2(float value);
AVMAPI AvmString AvmFtoa(double value);

AVMAPI AvmString AvmVSnprintf(const char8_t* format, size_t length,
                              va_list args);
AVMAPI AvmString AvmVSprintf(const char8_t* format, va_list args);
AVMAPI void AvmVFprintf(void* handle, const char8_t* format, va_list args);
AVMAPI void AvmVPrintf(const char8_t* format, va_list args);
AVMAPI void AvmVErrorf(const char8_t* format, va_list args);
AVMAPI void AvmScanf(const char8_t* format, ...);
AVMAPI void AvmSscanf(AvmString string, const char8_t* format, ...);

inline AvmString AvmSnprintf(const char8_t* format, size_t length, ...) {
    va_list args;
    va_start(args, length);
    AvmString s = AvmVSnprintf(format, length, args);
    va_end(args);
    return s;
}

inline AvmString AvmSprintf(const char8_t* format, ...) {
    va_list args;
    va_start(args, format);
    AvmString s = AvmVSprintf(format, args);
    va_end(args);
    return s;
}

inline void AvmFprintf(void* handle, const char8_t* format, ...) {
    va_list args;
    va_start(args, format);
    AvmVFprintf(handle, format, args);
    va_end(args);
}

inline void AvmPrintf(const char8_t* format, ...) {
    va_list args;
    va_start(args, format);
    AvmVPrintf(format, args);
    va_end(args);
}

inline void AvmErrorf(const char8_t* format, ...) {
    va_list args;
    va_start(args, format);
    AvmVErrorf(format, args);
    va_end(args);
}

#endif  // AVIUM_FMT_H
