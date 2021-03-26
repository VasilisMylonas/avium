#ifndef AVIUM_VALUE_H
#define AVIUM_VALUE_H

#include "avium/prologue.h"

typedef enum {
    ValueKindBool = 0,
    ValueKindInt,
    ValueKindUInt,
    ValueKindFloat,
    ValueKindStr,
    ValueKindChar,
} AvmValueKind;

AVM_CLASS(AvmValue, object, {
    union {
        bool _asBool;
        double _asFloat;
        ulong _asUInt;
        _long _asInt;
        str _asStr;
        char _asChar;
    };
    AvmValueKind _kind;
});

AVMAPI AvmValue AvmValueFromBool(bool value);
AVMAPI AvmValue AvmValueFromChar(char value);
AVMAPI AvmValue AvmValueFromFloat(double value);
AVMAPI AvmValue AvmValueFromUInt(ulong value);
AVMAPI AvmValue AvmValueFromInt(_long value);
AVMAPI AvmValue AvmValueFromStr(str value);

static_assert_s(sizeof(AvmValue) == 24);

#endif  // AVIUM_VALUE_H
