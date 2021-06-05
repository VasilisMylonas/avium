#include "avium/reflect.h"

#include "avium/string.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

#include <string.h>

void TestModuleHasSymbol()
{
    const AvmModule* module = AvmModuleGetCurrent();
    const str realSymbol = "AvmStringFrom";
    const str fakeSymbol = "AvmStringFromFake";

    assert(AvmModuleHasSymbol(module, realSymbol));
    assert(!AvmModuleHasSymbol(module, fakeSymbol));
}

void TestModuleGetCallback()
{
    const AvmModule* module = AvmModuleGetCurrent();
    const str name = "AvmModuleLoad";
    const AvmCallback callback = (AvmCallback)AvmModuleLoad;

    assert(callback == AvmModuleGetCallback(module, name));
}

void TestModuleGetType()
{
    const AvmModule* module = AvmModuleGetCurrent();
    const str typeName = "AvmString";
    const AvmType* const type = typeid(AvmString);

    assert(type == AvmModuleGetType(module, typeName));
}

void TestModuleGetName()
{
    const AvmModule* module = AvmModuleGetCurrent();
    const str moduleName = "reflect";

    assert(strcmp(moduleName, AvmModuleGetName(module)) == 0);
}

void TestReflectLoadType()
{
    const AvmType* const type = typeid(AvmLocation);
    const str qualifiedName = "avm.core@AvmLocation";

    assert(type == AvmReflectLoadType(qualifiedName));
}

void TestModuleGetVariable()
{
    // TODO
}

void Main()
{
    TestModuleHasSymbol();
    TestModuleGetCallback();
    TestModuleGetType();
    TestModuleGetName();
    TestReflectLoadType();
    TestModuleGetVariable();
}
