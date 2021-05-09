#include "avium/typeinfo.h"
#include "avium/error.h"
#include "avium/private/resources.h"

str AvmTypeGetName(const AvmType* self)
{
    if (self == NULL)
    {
        AvmPanic(SelfNullMsg);
    }

    return self->_name;
}

size_t AvmTypeGetSize(const AvmType* self)
{
    if (self == NULL)
    {
        AvmPanic(SelfNullMsg);
    }

    return self->_size;
}

AvmFunction AvmTypeGetFunction(const AvmType* self, size_t index)
{
    if (self == NULL)
    {
        AvmPanic(SelfNullMsg);
    }

    return self->_vptr[index];
}

const AvmType* AvmTypeGetBase(const AvmType* self)
{
    if (self == NULL)
    {
        AvmPanic(SelfNullMsg);
    }

    return self->_baseType;
}

bool AvmTypeInheritsFrom(const AvmType* self, const AvmType* baseType)
{
    if (self == NULL)
    {
        AvmPanic(SelfNullMsg);
    }

    if (baseType == NULL)
    {
        AvmPanic("Parameter `baseType` was `NULL`.");
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
