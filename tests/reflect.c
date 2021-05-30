#include "avium/reflect.h"
#include "avium/string.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

#include <string.h>

void main()
{
    const AvmModule* mod = AvmModuleGetCurrent();

    assert(AvmModuleHasSymbol(mod, "AvmStringFrom"));
    assert(!AvmModuleHasSymbol(mod, "AvmString"));

    AvmFunction func = AvmModuleGetFunction(mod, "AvmModuleLoad");
    assert(func == (AvmFunction)AvmModuleLoad);

    const AvmType* type = AvmModuleGetType(mod, "AvmString");
    assert(type == typeid(AvmString));

    str name = AvmModuleGetName(mod);
    assert(strcmp(name, "reflect") == 0);

    const AvmType* type2 = AvmReflectLoadType("avm.core@AvmLocation");
    assert(type2 == typeid(AvmLocation));
}
