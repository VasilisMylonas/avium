#include "avium/value.h"
#include "avium/fmt.h"
#include "avium/string.h"

static AvmString AvmValueToString(AvmValue* value) {
    switch (value->_kind) {
        case ValueKindBool:
            return AvmSprintf("%t", value->_asBool);
        case ValueKindChar:
            return AvmSprintf("%c", value->_asChar);
        case ValueKindFloat:
            return AvmSprintf("%f", value->_asFloat);
        case ValueKindInt:
            return AvmSprintf("%i", value->_asInt);
        case ValueKindUInt:
            return AvmSprintf("%u", value->_asUInt);
        case ValueKindStr:
            return AvmSprintf("%s", value->_asStr == NULL ? "" : value->_asStr);
        default:
            AvmPanic("Invalid AvmValue kind.");
    }
}

AVM_TYPE(AvmValue, {
                       [FUNC_DTOR] = NULL,
                       [FUNC_TO_STRING] = (AvmFunction)AvmValueToString,
                   });

AvmValue AvmValueFromBool(bool value) {
    return (AvmValue){
        ._asBool = value,
        ._kind = ValueKindBool,
        ._type = AVM_GET_TYPE(AvmValue),
    };
}

AvmValue AvmValueFromChar(char value) {
    return (AvmValue){
        ._asChar = value,
        ._kind = ValueKindChar,
        ._type = AVM_GET_TYPE(AvmValue),
    };
}

AvmValue AvmValueFromFloat(double value) {
    return (AvmValue){
        ._asFloat = value,
        ._kind = ValueKindFloat,
    };
}

AvmValue AvmValueFromUInt(ulong value) {
    return (AvmValue){
        ._asUInt = value,
        ._kind = ValueKindUInt,
        ._type = AVM_GET_TYPE(AvmValue),
    };
}

AvmValue AvmValueFromInt(_long value) {
    return (AvmValue){
        ._asInt = value,
        ._kind = ValueKindInt,
        ._type = AVM_GET_TYPE(AvmValue),
    };
}

AvmValue AvmValueFromStr(str value) {
    return (AvmValue){
        ._asStr = value,
        ._kind = ValueKindStr,
        ._type = AVM_GET_TYPE(AvmValue),
    };
}
