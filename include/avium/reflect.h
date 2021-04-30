#ifndef AVIUM_REFLECT_H
#define AVIUM_REFLECT_H

#include "avium/config.h"

#ifdef AVM_USE_REFLECT

#    include "avium/types.h"
#    include "avium/array-list.h"

typedef enum {
    SymbolTypeUnknown = 0,
    SymbolTypeVariable,
    SymbolTypeFunction,
    SymbolTypeType,
} AvmSymbolType;

AVM_CLASS(AvmModule, object, {
    void* _handle;
    str _name;
    AvmArrayList(AvmString) _symbols;
});

AVMAPI AvmModule AvmModuleLoad(str path);
AVMAPI AvmModule* AvmModuleGetCurrent(void);

AVMAPI str AvmModuleGetName(AvmModule* self);
AVMAPI size_t AvmModuleGetSymbolCount(AvmModule* self);
AVMAPI bool AvmModuleHasSymbol(AvmModule* self, str name);
AVMAPI AvmSymbolType AvmModuleGetSymbolType(AvmModule* self, str name);
AVMAPI AvmType* AvmModuleGetType(AvmModule* self, str name);
AVMAPI AvmFunction AvmModuleGetFunction(AvmModule* self, str name);
AVMAPI void* AvmModuleGetVariable(AvmModule* self, str name);

AVMAPI object AvmReflectConstructType(AvmType* type);

#endif  // AVM_USE_REFLECT

#endif  // AVIUM_REFLECT_H
