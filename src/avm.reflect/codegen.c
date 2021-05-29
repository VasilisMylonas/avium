#include "avium/codegen.h"
#include "avium/testing.h"

AVM_TYPE(AvmTypeBuilder, object, {[FnEntryDtor] = NULL});

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

void AvmTypeBuilderSetVFT(AvmTypeBuilder* self, uint length, AvmFunction vft[])
{
    pre
    {
        assert(self != NULL);
        assert(vft != NULL);
    }

    base->_vSize = length * sizeof(AvmFunction);
    base->_vPtr = vft;
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
