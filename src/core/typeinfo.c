#include "avium/typeinfo.h"

#include "avium/core.h"
#include "avium/string.h"
#include "avium/testing.h"

#include <stdlib.h>
#include <string.h>

#define _ AvmRuntimeGetResource

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
    (void)value;
    (void)type;
#ifdef AVM_THROW_ON_CAST_FAIL
    AvmString temp = AvmStringFormat(
        AVM_CAST_FAIL_FMT_STR, value, value, AvmTypeGetName(type));

    str msg = AvmStringToStr(&temp);

    throw(AvmErrorNew(msg));
#else
    return NULL;
#endif
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

    // We may be dealling with an AvmBox which has a fake type since it is just
    // a wrapper.
    if (self->_size <= sizeof(ulong))
    {
        return sizeof(ulong) + sizeof(const AvmType*);
    }

    return self->_size;
}

AvmCallback AvmTypeGetCallback(const AvmType* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    const uint length = self->_vSize / sizeof(AvmCallback);

    // If the index is valid then simply use that callback.
    if (index < length && self->_vPtr[index] != NULL)
    {
        return self->_vPtr[index];
    }

    // If the type is an object and the index is invalid then that mean that
    // there is no such callback in the inheritance hierarchy.
    if (self == typeid(object))
    {
        throw(AvmErrorNew(_(AvmMissingCallbackErrorMsg)));
    }

    // Otherwise we just keep looking up the chain.
    return AvmTypeGetCallback(self->_baseType, index);
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

    throw(AvmErrorNew(_(AvmMissingMemberErrorMsg)));
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

    throw(AvmErrorNew(_(AvmMissingMemberErrorMsg)));
}

uint AvmTypeGetMemberCount(const AvmType* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_mSize / sizeof(AvmMember);
}

static AvmString AvmTypeToString(const AvmType* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat("class %s (%u bytes)", self->_name, self->_size);
}

AVM_TYPE(AvmType, object, {[FnEntryToString] = (AvmCallback)AvmTypeToString});

str AvmEnumGetName(const AvmEnum* self)
{
    pre
    {
        assert(self != NULL);
    }

    return baseof(self)->_name;
}

uint AvmEnumGetSize(const AvmEnum* self)
{
    pre
    {
        assert(self != NULL);
    }

    return baseof(self)->_size;
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

    throw(AvmErrorNew(_(AvmMissingConstantErrorMsg)));
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

    throw(AvmErrorNew(_(AvmMissingConstantErrorMsg)));
}

static AvmString AvmEnumToString(const AvmEnum* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(
        "enum %s (%u bytes)", baseof(self)->_name, baseof(self)->_size);
}

AVM_TYPE(AvmEnum, AvmType, {[FnEntryToString] = (AvmCallback)AvmEnumToString});

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
             [FnEntryToString] = (AvmCallback)AvmMemberToString,
         });

const AvmType* AvmFunctionGetReturnType(const AvmFunction* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_returnType;
}

const AvmType** AvmFunctionGetParams(const AvmFunction* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_paramTypes;
}

uint AvmFunctionGetParamCount(const AvmFunction* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_paramCount;
}

str AvmFunctionGetName(const AvmFunction* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_name;
}

static AvmString AvmFunctionToString(const AvmFunction* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmString s = AvmStringNew(self->_paramCount * 4);
    AvmStringPushStr(&s, "function ");
    AvmStringPushStr(&s, self->_name);
    AvmStringPushStr(&s, " (");
    AvmStringPushUint(&s, self->_paramCount, AvmNumericBaseDecimal);
    if (self->_paramCount == 1)
    {
        AvmStringPushStr(&s, " parameter)");
    }
    else
    {
        AvmStringPushStr(&s, " parameters)");
    }
    return s;
}

AVM_TYPE(AvmFunction,
         object,
         {
             [FnEntryToString] = (AvmCallback)AvmFunctionToString,
         });

//
// Type info for primitive types.
//

static void objectFinalize(object self)
{
    pre
    {
        assert(self != NULL);
    }

    (void)self;
}

static bool objectEquals(object self, object other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    const size_t size = AvmTypeGetSize(AvmObjectGetType(self));
    return memcmp(self, other, size) == 0;
}

static object objectClone(object self)
{
    pre
    {
        assert(self != NULL);
    }

    const AvmType* type = AvmObjectGetType(self);
    object o = AvmObjectNew(type);
    AvmCopy(self, AvmTypeGetSize(type), (byte*)o);
    return o;
}

static AvmString objectToString(object self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(
        AVM_OBJECT_FMT_STR, AvmTypeGetName(AvmObjectGetType(self)), self);
}

static AvmString signedToString(const AvmBox* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromInt(self->AsInt);
}

static bool signedEquals(const AvmBox* self, const AvmBox* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return self->AsInt == other->AsInt;
}

static AvmString unsignedToString(const AvmBox* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromUint(self->AsUint, AvmNumericBaseDecimal);
}

static bool unsignedEquals(const AvmBox* self, const AvmBox* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return self->AsUint == other->AsUint;
}

static AvmString floatToString(const AvmBox* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromFloat(self->AsFloat, AvmFloatReprAuto);
}

static bool floatEquals(const AvmBox* self, const AvmBox* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return self->AsFloat == other->AsFloat;
}

static AvmString strToString(const AvmBox* self)
{
    pre
    {
        assert(self != NULL);
        assert(self->AsStr != NULL);
    }

    return AvmStringFrom(self->AsStr);
}

static bool strEquals(const AvmBox* self, const AvmBox* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
        assert(self->AsStr != NULL);
        assert(other->AsStr != NULL);
    }

    return strcmp(self->AsStr, other->AsStr) == 0;
}

// For consistency with other primitive types.
static AvmString voidToString()
{
    return AvmStringNew(0);
}

static bool voidEquals()
{
    return false;
}

AVM_TYPE(object,
         object,
         {
             [FnEntryFinalize] = (AvmCallback)objectFinalize,
             [FnEntryEquals] = (AvmCallback)objectEquals,
             [FnEntryClone] = (AvmCallback)objectClone,
             [FnEntryToString] = (AvmCallback)objectToString,
         });

AVM_TYPE(_long,
         object,
         {
             [FnEntryToString] = (AvmCallback)signedToString,
             [FnEntryEquals] = (AvmCallback)signedEquals,
         });

AVM_TYPE(ulong,
         object,
         {
             [FnEntryToString] = (AvmCallback)unsignedToString,
             [FnEntryEquals] = (AvmCallback)unsignedEquals,
         });

AVM_TYPE(int,
         object,
         {
             [FnEntryToString] = (AvmCallback)signedToString,
             [FnEntryEquals] = (AvmCallback)signedEquals,
         });

AVM_TYPE(uint,
         object,
         {
             [FnEntryToString] = (AvmCallback)unsignedToString,
             [FnEntryEquals] = (AvmCallback)unsignedEquals,
         });

AVM_TYPE(short,
         object,
         {
             [FnEntryToString] = (AvmCallback)signedToString,
             [FnEntryEquals] = (AvmCallback)signedEquals,
         });

AVM_TYPE(ushort,
         object,
         {
             [FnEntryToString] = (AvmCallback)unsignedToString,
             [FnEntryEquals] = (AvmCallback)unsignedEquals,
         });

AVM_TYPE(char,
         object,
         {
             [FnEntryToString] = (AvmCallback)signedToString,
             [FnEntryEquals] = (AvmCallback)signedEquals,
         });

AVM_TYPE(byte,
         object,
         {
             [FnEntryToString] = (AvmCallback)unsignedToString,
             [FnEntryEquals] = (AvmCallback)unsignedEquals,
         });

AVM_TYPE(str,
         object,
         {
             [FnEntryToString] = (AvmCallback)strToString,
             [FnEntryEquals] = (AvmCallback)strEquals,
         });

AVM_TYPE(float,
         object,
         {
             [FnEntryToString] = (AvmCallback)floatToString,
             [FnEntryEquals] = (AvmCallback)floatEquals,
         });

AVM_TYPE(double,
         object,
         {
             [FnEntryToString] = (AvmCallback)floatToString,
             [FnEntryEquals] = (AvmCallback)floatEquals,
         });

// Because we can't sizeof(void) we have to do this manually.
static AvmCallback AVM_VT_NAME(void)[] = {
    [FnEntryToString] = (AvmCallback)voidToString,
    [FnEntryEquals] = (AvmCallback)voidEquals,
};

const AvmType AVM_TI_NAME(void) = {
    ._type = typeid(AvmType),
    ._vPtr = AVM_VT_NAME(void),
    ._name = "void",
    ._baseType = typeid(object),
    ._vSize = sizeof(AVM_VT_NAME(void)),
    ._size = 0,
};
