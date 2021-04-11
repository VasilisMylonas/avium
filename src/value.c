#include "avium/value.h"  // Public interface.

#include "avium/fmt.h"        // For AvmSprintf
#include "avium/resources.h"  // For panic messages.
#include "avium/runtime.h"    // For AVM_GET_TYPE, AVM_TYPE, AvmPanic, AvmAlloc

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

#define VALUE_FROM(kind, member)                                         \
    return (AvmValue) {                                                  \
        ._type = AVM_GET_TYPE(AvmValue), .member = value, ._kind = kind, \
    }

#define BUILD_FMT_STR(format) \
    (const char[]) { '%', format, '\0' }

static object AvmValueClone(AvmValue* value) {
    AvmValue* copy = AvmAlloc(sizeof(AvmValue));
    AvmMemCopy((byte*)value, sizeof(AvmValue), (byte*)copy, sizeof(AvmValue));
    return copy;
}

static AvmString AvmValueToString(AvmValue* value) {
    switch (value->_kind) {
        case ValueKindBool:
            return AvmSprintf(BUILD_FMT_STR(AVM_FMT_BOOL), value->_asBool);
        case ValueKindChar:
            return AvmSprintf(BUILD_FMT_STR(AVM_FMT_CHAR), value->_asChar);
        case ValueKindFloat:
            return AvmSprintf(BUILD_FMT_STR(AVM_FMT_FLOAT), value->_asFloat);
        case ValueKindInt:
            return AvmSprintf(BUILD_FMT_STR(AVM_FMT_INT_DECIMAL),
                              value->_asInt);
        case ValueKindUInt:
            return AvmSprintf(BUILD_FMT_STR(AVM_FMT_INT_UNSIGNED),
                              value->_asUInt);
        case ValueKindStr:
            return AvmSprintf(BUILD_FMT_STR(AVM_FMT_STRING),
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
                       [FUNC_TO_STRING] = (AvmFunction)AvmValueToString,
                       [FUNC_CLONE] = (AvmFunction)AvmValueClone,
                       [FUNC_EQUALS] = (AvmFunction)AvmValueEquals,
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
