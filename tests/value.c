#include <avium/testing.h>
#include <avium/value.h>

static object TestInit() { return NULL; }

__test TestBool(object state) {
    (void)state;

    AvmValue value = AvmValueFromBool(true);
    AssertEqual(AvmValueGetKind(&value), ValueKindBool);
    AssertEqual(AvmValueAsBool(&value), true);
}

__test TestFloat(object state) {
    (void)state;

    AvmValue value = AvmValueFromFloat(0.5);
    AssertEqual(AvmValueGetKind(&value), ValueKindFloat);
    AssertEqual(AvmValueAsFloat(&value), 0.5);
}

__test TestChar(object state) {
    (void)state;

    AvmValue value = AvmValueFromChar('a');
    AssertEqual(AvmValueGetKind(&value), ValueKindChar);
    AssertEqual(AvmValueAsChar(&value), 'a');
}

__test TestUInt(object state) {
    (void)state;

    AvmValue value = AvmValueFromUInt(36);
    AssertEqual(AvmValueGetKind(&value), ValueKindUInt);
    AssertEqual(AvmValueAsUInt(&value), 36);
}

__test TestInt(object state) {
    (void)state;

    AvmValue value = AvmValueFromInt(36);
    AssertEqual(AvmValueGetKind(&value), ValueKindInt);
    AssertEqual(AvmValueAsInt(&value), 36);
}

__test TestStr(object state) {
    (void)state;

    AvmValue value = AvmValueFromStr("Hello");
    AssertEqual(AvmValueGetKind(&value), ValueKindStr);
    AssertStrEqual(AvmValueAsStr(&value), "Hello");
}
