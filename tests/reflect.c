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

void TestModuleGetFunction()
{
    const AvmModule* module = AvmModuleGetCurrent();
    const str functionName = "AvmModuleLoad";
    const AvmFunction function = (AvmFunction)AvmModuleLoad;

    assert(function == AvmModuleGetFunction(module, functionName));
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

void main()
{
    TestModuleHasSymbol();
    TestModuleGetFunction();
    TestModuleGetType();
    TestModuleGetName();
    TestReflectLoadType();
    TestModuleGetVariable();
}
