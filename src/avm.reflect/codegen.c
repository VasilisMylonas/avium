#include "avium/codegen.h"

#include "avium/private/resources.h"

#include <libtcc.h>

static void AvmFnBuilderDestroy(AvmFnBuilder* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    AvmObjectDestroy(&self->_source);
    AvmObjectDestroy(&self->_params);
    tcc_delete(self->_state);
    AvmDealloc(self);
}

AVM_TYPE(AvmFnBuilder, object,
         {[FnEntryDtor] = (AvmFunction)AvmFnBuilderDestroy});

AvmFnBuilder* AvmFnBuilderNew(size_t length, ...) {
    AvmFnBuilder* builder = AvmAlloc(sizeof(AvmFnBuilder));
    builder->_type = typeid(AvmFnBuilder);
    builder->_source = AvmStringNew(256 + length * 16);
    builder->_params = AvmStringNew(32);
    builder->_state = tcc_new();
    builder->_returnType = "void";
    builder->_name = NULL;
    builder->_body = NULL;

    va_list args;
    va_start(args, length);

    for (size_t i = 0; i < length; i++) {
        AvmStringPushStr(&builder->_source, "#include <");
        AvmStringPushStr(&builder->_source, va_arg(args, str));
        AvmStringPushStr(&builder->_source, ">\n");
    }

    va_end(args);

    tcc_set_output_type(builder->_state, TCC_OUTPUT_MEMORY);

    return builder;
}

void AvmFnBuilderSetReturnType(AvmFnBuilder* self, str type) {
    AvmStringPushStr(&self->_source, type);
    AvmStringPushChar(&self->_source, ' ');
}

void AvmFnBuilderSetName(AvmFnBuilder* self, str name) { self->_name = name; }

void AvmFnBuilderAddParam(AvmFnBuilder* self, str type, str name) {
    AvmStringPushStr(&self->_params, type);
    AvmStringPushChar(&self->_params, ' ');
    AvmStringPushStr(&self->_params, name);
    AvmStringPushChar(&self->_params, ',');
}

void AvmFnBuilderAddBody(AvmFnBuilder* self, str code) { self->_body = code; }

AvmFunction AvmFnBuilderCompile(AvmFnBuilder* self) {
    AvmStringPushStr(&self->_source, self->_returnType);
    AvmStringPushChar(&self->_source, ' ');
    AvmStringPushStr(&self->_source, self->_name);
    AvmStringPushChar(&self->_source, '(');
    AvmStringPushString(&self->_source, &self->_params);
    AvmStringReplaceLast(&self->_source, ',', ' ');
    char* buffer = AvmStringAsPtr(&self->_source);
    AvmStringPushChar(&self->_source, ')');
    AvmStringPushStr(&self->_source, "{\n");
    AvmStringPushStr(&self->_source, self->_body);
    AvmStringPushStr(&self->_source, "\n}");

    tcc_compile_string(self->_state, buffer);
    tcc_relocate(self->_state, TCC_RELOCATE_AUTO);

    void* symbol = tcc_get_symbol(self->_state, self->_name);
    return *(AvmFunction*)&symbol;
}

AVM_TYPE(AvmTypeBuilder, object, {[FnEntryDtor] = NULL});

AvmTypeBuilder* AvmTypeBuilderNew(AvmType* baseType) {
    AvmTypeBuilder* self = AvmAlloc(sizeof(AvmTypeBuilder));
    self->_type = typeid(AvmTypeBuilder);
    base->_baseType = baseType;
    base->_size = AvmTypeGetSize(baseType);
    base->_vptr[FnEntryDtor] = NULL;
    return self;
}

void AvmTypeBuilderSetName(AvmTypeBuilder* self, str name) {
    base->_name = name;
}

void AvmTypeBuilderAddMember(AvmTypeBuilder* self, AvmType* type) {
    base->_size += AvmTypeGetSize(type);
}

void AvmTypeBuilderSetDtor(AvmTypeBuilder* self, AvmFunction dtor) {
    base->_vptr[FnEntryDtor] = dtor;
}

AVMAPI AvmType* AvmTypeBuilderCompile(AvmTypeBuilder* self) {
    AvmType* type = heapalloc(AvmType, {
                                           ._type = typeid(AvmType),
                                           ._baseType = base->_baseType,
                                           ._name = base->_name,
                                           ._size = base->_size,
                                       });

    type->_vptr[FnEntryDtor] = base->_vptr[FnEntryDtor];
    return type;
}
