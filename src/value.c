#include "avium/value.h"
#include "avium/fmt.h"
#include "avium/resources.h"
#include <stdlib.h>

static object AvmValueClone(AvmValue* value) {
    AvmValue* copy = malloc(sizeof(AvmValue));
    AvmObjectCopy(value, sizeof(AvmValue), (byte*)copy);
    return copy;
}

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
                       [FUNC_CLONE] = (AvmFunction)(AvmValueClone),
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

bool AvmValueAsBool(AvmValue* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self->_kind != ValueKindBool) {
        AvmPanic(InvalidValueCastMsg);
    }

    return self->_asBool;
}

char AvmValueAsChar(AvmValue* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self->_kind != ValueKindChar) {
        AvmPanic(InvalidValueCastMsg);
    }

    return self->_asChar;
}

double AvmValueAsFloat(AvmValue* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self->_kind != ValueKindFloat) {
        AvmPanic(InvalidValueCastMsg);
    }

    return self->_asFloat;
}

ulong AvmValueAsUInt(AvmValue* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self->_kind != ValueKindUInt) {
        AvmPanic(InvalidValueCastMsg);
    }

    return self->_asUInt;
}

_long AvmValueAsInt(AvmValue* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self->_kind != ValueKindInt) {
        AvmPanic(InvalidValueCastMsg);
    }

    return self->_asInt;
}

str AvmValueAsStr(AvmValue* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (self->_kind != ValueKindStr) {
        AvmPanic(InvalidValueCastMsg);
    }

    return self->_asStr;
}

AvmValueKind AvmValueGetKind(AvmValue* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_kind;
}
