#include "avium/typeinfo.h"

#include "avium/error.h"
#include "avium/testing.h"

str AvmTypeGetName(const AvmType* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_name;
}

uint AvmTypeGetSize(const AvmType* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_size;
}

AvmFunction AvmTypeGetFunction(const AvmType* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_vptr[index];
}

const AvmType* AvmTypeGetBase(const AvmType* self)
{
    pre
    {
        assert(self != NULL);
    }
    return self->_baseType;
}

bool AvmTypeInheritsFrom(const AvmType* self, const AvmType* baseType)
{
    pre
    {
        assert(self != NULL);
        assert(baseType != NULL);
    }

    if (baseType == typeid(object))
    {
        return true;
    }

    for (const AvmType* temp = AvmTypeGetBase(self); temp != typeid(object);
         temp = AvmTypeGetBase(temp))
    {
        if (temp == baseType)
        {
            return true;
        }
    }

    return false;
}

AVM_TYPE(AvmType, object, {[FnEntryDtor] = NULL});
