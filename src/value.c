#include "avium/value.h"

#include "avium/string.h"     // For AvmStringFormat
#include "avium/resources.h"  // For panic messages.
#include "avium/runtime.h"    // For typeid, AVM_TYPE, AvmPanic, AvmAlloc

#include <string.h>  // For strcmp

#define VALUE_AS(kind, member)         \
    if (self == NULL) {                \
        AvmPanic(SelfNullMsg);         \
    }                                  \
                                       \
    if (self->_kind != kind) {         \
        AvmPanic(InvalidValueCastMsg); \
    }                                  \
                                       \
    return self->member

#define VALUE_FROM(kind, member)                                   \
    return (AvmValue) {                                            \
        ._type = typeid(AvmValue), .member = value, ._kind = kind, \
    }

static object AvmValueClone(AvmValue* value) {
    AvmValue* copy = AvmAlloc(sizeof(AvmValue));
    AvmMemCopy((byte*)value, sizeof(AvmValue), (byte*)copy, sizeof(AvmValue));
    return copy;
}

static AvmString AvmValueToString(AvmValue* value) {
    switch (value->_kind) {
        case ValueKindBool:
            return AvmStringFormat("%t", value->_asBool);
        case ValueKindChar:
            return AvmStringFormat("%c", value->_asChar);
        case ValueKindFloat:
            return AvmStringFormat("%f", value->_asFloat);
        case ValueKindInt:
            return AvmStringFormat("%i", value->_asInt);
        case ValueKindUInt:
            return AvmStringFormat("%u", value->_asUInt);
        case ValueKindStr:
            return AvmStringFormat("%s",
                                   value->_asStr == NULL ? "" : value->_asStr);
        default:
            AvmPanic(InvalidValueKindMsg);
    }
}

static bool AvmValueEquals(AvmValue* self, AvmValue* other) {
    if (self->_kind != other->_kind) {
        return false;
    }

    switch (self->_kind) {
        case ValueKindBool:
            return self->_asBool == other->_asBool;
        case ValueKindChar:
            return self->_asChar == other->_asChar;
        case ValueKindFloat:
            return self->_asFloat == other->_asFloat;
        case ValueKindInt:
            return self->_asInt == other->_asInt;
        case ValueKindUInt:
            return self->_asUInt == other->_asUInt;
        case ValueKindStr:
            return strcmp(self->_asStr, other->_asStr) == 0;
        default:
            AvmPanic(InvalidValueKindMsg);
    }
}

AVM_TYPE(AvmValue, {
                       [FnEntryToString] = (AvmFunction)AvmValueToString,
                       [FnEntryClone] = (AvmFunction)AvmValueClone,
                       [FnEntryEquals] = (AvmFunction)AvmValueEquals,
                   });

AvmValue AvmValueFromBool(bool value) { VALUE_FROM(ValueKindBool, _asBool); }
AvmValue AvmValueFromChar(char value) { VALUE_FROM(ValueKindChar, _asChar); }
AvmValue AvmValueFromUInt(ulong value) { VALUE_FROM(ValueKindUInt, _asUInt); }
AvmValue AvmValueFromInt(_long value) { VALUE_FROM(ValueKindInt, _asInt); }
AvmValue AvmValueFromStr(str value) { VALUE_FROM(ValueKindStr, _asStr); }
AvmValue AvmValueFromFloat(double value) {
    VALUE_FROM(ValueKindFloat, _asFloat);
}

bool AvmValueAsBool(AvmValue* self) { VALUE_AS(ValueKindBool, _asBool); }
char AvmValueAsChar(AvmValue* self) { VALUE_AS(ValueKindChar, _asChar); }
double AvmValueAsFloat(AvmValue* self) { VALUE_AS(ValueKindFloat, _asFloat); }
ulong AvmValueAsUInt(AvmValue* self) { VALUE_AS(ValueKindUInt, _asUInt); }
_long AvmValueAsInt(AvmValue* self) { VALUE_AS(ValueKindInt, _asInt); }
str AvmValueAsStr(AvmValue* self) { VALUE_AS(ValueKindStr, _asStr); }

AvmValueKind AvmValueGetKind(AvmValue* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_kind;
}
