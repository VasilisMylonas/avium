#include "avium/typeinfo.h"

#include "avium/core.h"
#include "avium/private/errors.h"
#include "avium/string.h"
#include "avium/testing.h"

#include <stdlib.h>
#include <string.h>

object __AvmRuntimeCastFail(object value, const AvmType* type)
{
    AvmString temp =
        AvmStringFormat("Tried to cast object [%x] of type %T to type %s.",
                        value,
                        value,
                        AvmTypeGetName(type));

    str msg = AvmStringToStr(&temp);
    AvmObjectDestroy(&temp);

    throw(AvmErrorNew(msg));
}

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

    // TODO!!!
    // BUG: very likely
    if (index < self->_vSize)
    {
        if (self->_vPtr[index] != NULL || self == typeid(object))
        {
            return self->_vPtr[index];
        }

        return AvmTypeGetFunction(self->_baseType, index);
    }

    throw(AvmErrorNew(VirtualFuncError));
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

object AvmTypeConstruct(const AvmType* self)
{
    pre
    {
        assert(self != NULL);
    }

    object o = AvmAlloc(self->_size);
    *(const AvmType**)o = self;
    return o;
}

static AvmString AvmTypeToString(AvmType* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat("class %s (%u bytes)", self->_name, self->_size);
}

AVM_TYPE(AvmType, object, {[FnEntryToString] = (AvmFunction)AvmTypeToString});

str AvmEnumGetName(const AvmEnum* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_name;
}

uint AvmEnumGetSize(const AvmEnum* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_size;
}

bool AvmEnumIsDefined(const AvmEnum* self, _long value)
{
    pre
    {
        assert(self != NULL);
    }

    for (uint i = 0; true; i++)
    {
        if (self->_members[i]._value == 0 && self->_members[i]._name == NULL)
        {
            break;
        }

        if (self->_members[i]._value == value)
        {
            return true;
        }
    }

    return false;
}

str AvmEnumGetNameOf(const AvmEnum* self, _long value)
{
    pre
    {
        assert(self != NULL);
    }

    for (uint i = 0; true; i++)
    {
        if (self->_members[i]._value == 0 && self->_members[i]._name == NULL)
        {
            break;
        }

        if (self->_members[i]._value == value)
        {
            return self->_members[i]._name;
        }
    }

    throw(AvmErrorNew(EnumConstantNotPresentError));
}

_long AvmEnumGetValueOf(const AvmEnum* self, str name)
{
    pre
    {
        assert(self != NULL);
        assert(name != NULL);
    }

    for (uint i = 0; true; i++)
    {
        if (self->_members[i]._value == 0 && self->_members[i]._name == NULL)
        {
            break;
        }

        if (strcmp(self->_members[i]._name, name) == 0)
        {
            return self->_members[i]._value;
        }
    }

    throw(AvmErrorNew(EnumConstantNotPresentError));
}

static AvmString AvmEnumToString(AvmEnum* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat("enum %s (%u bytes)", self->_name, self->_size);
}

AVM_TYPE(AvmEnum, object, {[FnEntryToString] = (AvmFunction)AvmEnumToString});
