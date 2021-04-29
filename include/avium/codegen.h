#ifndef AVIUM_CODEGEN_H
#define AVIUM_CODEGEN_H

#include "avium/types.h"
#include "avium/string.h"
#include "avium/reflect.h"

AVM_CLASS(AvmFnBuilder, object, {
    void* _state;
    str _returnType;
    str _name;
    str _body;
    AvmString _params;
    AvmString _source;
});

#define weak(T) T*

AVMAPI AvmFnBuilder* AvmFnBuilderNew(size_t length, ...);
AVMAPI void AvmFnBuilderSetReturnType(AvmFnBuilder* self, str type);
AVMAPI void AvmFnBuilderSetName(AvmFnBuilder* self, str name);
AVMAPI void AvmFnBuilderAddParam(AvmFnBuilder* self, str type, str name);
AVMAPI void AvmFnBuilderAddBody(AvmFnBuilder* self, str code);
AVMAPI AvmFunction AvmFnBuilderCompile(AvmFnBuilder* self);

AVM_CLASS(AvmTypeBuilder, AvmType, { size_t _reserved; });

AVMAPI AvmTypeBuilder* AvmTypeBuilderNew(AvmType* baseType);
AVMAPI void AvmTypeBuilderSetName(AvmTypeBuilder* self, str name);
AVMAPI void AvmTypeBuilderSetDtor(AvmTypeBuilder* self, AvmFunction dtor);
AVMAPI void AvmTypeBuilderAddMember(AvmTypeBuilder* self, AvmType* type);

AVMAPI AvmType* AvmTypeBuilderCompile(AvmTypeBuilder* self);
#endif  // AVIUM_CODEGEN_H
