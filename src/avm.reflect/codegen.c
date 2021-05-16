#include "avium/codegen.h"
#include "avium/testing.h"

static void AvmTypeBuilderDestroy(AvmTypeBuilder* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmDealloc(self);
}

AVM_TYPE(AvmTypeBuilder,
         object,
         {[FnEntryDtor] = (AvmFunction)AvmTypeBuilderDestroy});

AvmTypeBuilder* AvmTypeBuilderBasedOn(const AvmType* type)
{
    pre
    {
        assert(type != NULL);
    }

    AvmTypeBuilder* self = AvmAlloc(sizeof(AvmTypeBuilder));
    self->_type = typeid(AvmTypeBuilder);
    base->_baseType = type;
    base->_size = AvmTypeGetSize(type);
    base->_vPtr[FnEntryDtor] = NULL;
    return self;
}

void AvmTypeBuilderSetName(AvmTypeBuilder* self, str name)
{
    pre
    {
        assert(self != NULL);
        assert(name != NULL);
    }

    base->_name = name;
}

void AvmTypeBuilderSetDtor(AvmTypeBuilder* self, AvmDtor dtor)
{
    pre
    {
        assert(self != NULL);
        assert(dtor != NULL);
    }

    base->_vPtr[FnEntryDtor] = (AvmFunction)dtor;
}

void AvmTypeBuilderAddMember(AvmTypeBuilder* self, const AvmType* type)
{
    pre
    {
        assert(self != NULL);
        assert(type != NULL);
    }

    base->_size += AvmTypeGetSize(type);
}
