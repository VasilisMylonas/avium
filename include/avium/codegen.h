#ifndef AVIUM_CODEGEN_H
#define AVIUM_CODEGEN_H

#include "avium/config.h"

#ifdef AVM_USE_REFLECT

#    include "avium/types.h"

#    ifdef AVM_HAVE_LIBTCC_H
AVM_CLASS(AvmFnBuilder, object, {
    void* _state;
    str _returnType;
    str _name;
    str _body;
    AvmString _params;
    AvmString _source;
});

AVMAPI AvmFnBuilder* AvmFnBuilderNew(size_t length, ...);
AVMAPI void AvmFnBuilderSetReturnType(AvmFnBuilder* self, str type);
AVMAPI void AvmFnBuilderSetName(AvmFnBuilder* self, str name);
AVMAPI void AvmFnBuilderAddParam(AvmFnBuilder* self, str type, str name);
AVMAPI void AvmFnBuilderAddBody(AvmFnBuilder* self, str code);
AVMAPI AvmFunction AvmFnBuilderCompile(AvmFnBuilder* self);
#    endif  // AVM_HAVE_LIBTCC_H

AVM_CLASS(AvmTypeBuilder, AvmType, { size_t _reserved; });

AVMAPI AvmTypeBuilder* AvmTypeBuilderNew(AvmType* baseType);
AVMAPI void AvmTypeBuilderSetName(AvmTypeBuilder* self, str name);
AVMAPI void AvmTypeBuilderSetDtor(AvmTypeBuilder* self, AvmFunction dtor);
AVMAPI void AvmTypeBuilderAddMember(AvmTypeBuilder* self, AvmType* type);

AVMAPI AvmType* AvmTypeBuilderCompile(AvmTypeBuilder* self);

#endif  // AVM_USE_REFLECT

#endif  // AVIUM_CODEGEN_H
