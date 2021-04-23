#ifndef AVIUM_REFLECT_H
#define AVIUM_REFLECT_H

#include "avium/types.h"
#include "avium/array-list.h"

#ifdef AVM_HAVE_DLFCN_H
typedef enum {
    SymbolTypeUnknown = 0,
    SymbolTypeVariable,
    SymbolTypeFunction,
    SymbolTypeType,
} AvmSymbolType;

AVM_CLASS(AvmSharedObject, object, {
    void* _handle;
    AvmArrayList(AvmString) _symbols;
});

AVMAPI AvmSharedObject AvmReflectLoadObject(str path);
AVMAPI size_t AvmReflectGetSymbolCount(AvmSharedObject* self);
AVMAPI AvmSymbolType AvmReflectGetSymbolType(AvmSharedObject* self, str name);

AVMAPI AvmType* AvmReflectGetType(AvmSharedObject* self, str name);
AVMAPI AvmFunction AvmReflectGetFunction(AvmSharedObject* self, str name);
AVMAPI void* AvmReflectGetVariable(AvmSharedObject* self, str name);
#endif  // AVM_HAVE_DLFCN_H

#endif  // AVIUM_REFLECT_H
