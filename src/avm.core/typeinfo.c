#include "avium/typeinfo.h"

#include "avium/core.h"
#include "avium/private/constants.h"
#include "avium/string.h"
#include "avium/testing.h"

#include <stdlib.h>
#include <string.h>

AVM_ENUM_TYPE(AvmFnEntry,
              {
                  AVM_ENUM_MEMBER(FnEntryFinalize),
                  AVM_ENUM_MEMBER(FnEntryToString),
                  AVM_ENUM_MEMBER(FnEntryClone),
                  AVM_ENUM_MEMBER(FnEntryEquals),
                  AVM_ENUM_MEMBER(FnEntryRead),
                  AVM_ENUM_MEMBER(FnEntryWrite),
                  AVM_ENUM_MEMBER(FnEntrySeek),
                  AVM_ENUM_MEMBER(FnEntryFlush),
                  AVM_ENUM_MEMBER(FnEntryGetPosition),
                  AVM_ENUM_MEMBER(FnEntryGetLength),
                  AVM_ENUM_MEMBER(FnEntryGetCapacity),
                  AVM_ENUM_MEMBER(FnEntryRemove),
                  AVM_ENUM_MEMBER(FnEntryInsert),
                  AVM_ENUM_MEMBER(FnEntryItemAt),
                  AVM_ENUM_MEMBER(FnEntryGetItemType),
              });

object __AvmRuntimeCastFail(object value, const AvmType* type)
{
    AvmString temp =
        AvmStringFormat(CAST_FAIL_STR, value, value, AvmTypeGetName(type));

    str msg = AvmStringToStr(&temp);

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

    if (self == typeid(object) &&
        (index > (self->_vSize / sizeof(AvmFunction)) ||
         self->_vPtr[index] == NULL))
    {
        throw(AvmErrorNew(VirtualFuncError));
    }

    // TODO!!!
    // BUG: very likely
    if (index < (self->_vSize / sizeof(AvmFunction)))
    {
        if (self->_vPtr[index] != NULL || self == typeid(object))
        {
            return self->_vPtr[index];
        }
    }

    return AvmTypeGetFunction(self->_baseType, index);
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

static AvmString AvmTypeToString(const AvmType* self)
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

    return base->_name;
}

uint AvmEnumGetSize(const AvmEnum* self)
{
    pre
    {
        assert(self != NULL);
    }

    return base->_size;
}

bool AvmEnumIsDefined(const AvmEnum* self, _long value)
{
    pre
    {
        assert(self != NULL);
    }

    for (uint i = 0; i < (self->_cSize / (sizeof(_long) + sizeof(str))); i++)
    {
        if (self->_cPtr[i]._value == value)
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
        if (self->_cPtr[i]._value == 0 && self->_cPtr[i]._name == NULL)
        {
            break;
        }

        if (self->_cPtr[i]._value == value)
        {
            return self->_cPtr[i]._name;
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
        if (self->_cPtr[i]._value == 0 && self->_cPtr[i]._name == NULL)
        {
            break;
        }

        if (strcmp(self->_cPtr[i]._name, name) == 0)
        {
            return self->_cPtr[i]._value;
        }
    }

    throw(AvmErrorNew(EnumConstantNotPresentError));
}

static AvmString AvmEnumToString(const AvmEnum* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat("enum %s (%u bytes)", base->_name, base->_size);
}

AVM_TYPE(AvmEnum, AvmType, {[FnEntryToString] = (AvmFunction)AvmEnumToString});

const AvmMember* AvmTypeGetMemberAt(const AvmType* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    if (index < self->_mSize / sizeof(AvmMember))
    {
        return &self->_mPtr[index];
    }

    throw(AvmErrorNew(MemberNotPresentError));
}

const AvmMember* AvmTypeGetMember(const AvmType* self, str name)
{
    pre
    {
        assert(self != NULL);
        assert(name != NULL);
    }

    for (uint i = 0; i < (self->_mSize / sizeof(AvmMember)); i++)
    {
        if (strcmp(self->_mPtr[i]._name, name) == 0)
        {
            return &self->_mPtr[i];
        }
    }

    throw(AvmErrorNew(MemberNotPresentError));
}

uint AvmTypeGetMemberCount(const AvmType* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_mSize / sizeof(AvmMember);
}

uint AvmMemberGetOffset(const AvmMember* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_offset;
}

str AvmMemberGetName(const AvmMember* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_name;
}

static AvmString AvmMemberToString(const AvmMember* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(
        "member %s (%u bytes offset)", self->_name, self->_offset);
}

AVM_TYPE(AvmMember,
         object,
         {
             [FnEntryToString] = (AvmFunction)AvmMemberToString,
         });
