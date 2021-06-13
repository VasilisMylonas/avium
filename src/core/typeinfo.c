#include "avium/typeinfo.h"

#include "avium/core.h"
#include "avium/string.h"

#include <stdlib.h>
#include <string.h>

#define _ AvmRuntimeGetResource

object __AvmRuntimeCastFail(object value, const AvmClass* type)
{
    (void)value;
    (void)type;
#ifdef AVM_THROW_ON_CAST_FAIL
    AvmString temp = AvmStringFormat(AVM_CAST_FAIL_FMT_STR,
                                     value,
                                     value,
                                     AvmTypeGetName((const AvmType*)type));

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

    return self->_private.name;
}

uint AvmTypeGetSize(const AvmType* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_private.size;
}

const byte* AvmTypeGetInit(const AvmType* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_private.init;
}

static AvmString AvmTypeToString(const AvmType* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(
        "type %s (%u bytes)", self->_private.name, self->_private.size);
}

AVM_CLASS_TYPE(AvmType,
               object,
               {
                   [FnEntryToString] = (AvmCallback)AvmTypeToString,
               });

AvmCallback AvmClassGetCallback(const AvmClass* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    const uint length = self->_private.vCount;

    // If the index is valid then simply use that callback.
    if (index < length && self->_private.vPtr[index] != NULL)
    {
        return self->_private.vPtr[index];
    }

    // If the type is an object and the index is invalid then that mean that
    // there is no such callback in the inheritance hierarchy.
    if (self == typeid(object))
    {
        throw(AvmErrorNew(_(AvmMissingCallbackErrorMsg)));
    }

    // Otherwise we just keep looking up the chain.
    return AvmClassGetCallback(self->_private.base, index);
}

const AvmClass* AvmClassGetBase(const AvmClass* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_private.base;
}

bool AvmClassInheritsFrom(const AvmClass* self, const AvmClass* baseType)
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

    for (const AvmClass* temp = AvmClassGetBase(self); temp != typeid(object);
         temp = AvmClassGetBase(temp))
    {
        if (temp == baseType)
        {
            return true;
        }
    }

    return false;
}

const AvmMember* AvmClassGetMemberAt(const AvmClass* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    if (index < self->_private.memberCount)
    {
        return &self->_private.members[index];
    }

    throw(AvmErrorNew(_(AvmMissingMemberErrorMsg)));
}

const AvmMember* AvmClassGetMember(const AvmClass* self, str name)
{
    pre
    {
        assert(self != NULL);
        assert(name != NULL);
    }

    for (uint i = 0; i < self->_private.memberCount; i++)
    {
        if (strcmp(self->_private.members[i].__base._private.name, name) == 0)
        {
            return &self->_private.members[i];
        }
    }

    throw(AvmErrorNew(_(AvmMissingMemberErrorMsg)));
}

uint AvmClassGetMemberCount(const AvmClass* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_private.memberCount;
}

const AvmInterface* AvmClassGetInterface(const AvmClass* self, str name)
{
    const uint count = self->_private.interfaceCount;
    const AvmInterface** interfaces = self->_private.interfaces;

    for (uint i = 0; i < count; i++)
    {
        if (strcmp(interfaces[i]->__base._private.name, name) == 0)
        {
            return interfaces[i];
        }
    }

    return NULL;
}

static AvmString AvmClassToString(const AvmClass* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat("type %s (%u bytes)",
                           AvmTypeGetName(&self->__base),
                           AvmTypeGetSize(&self->__base));
}

AVM_CLASS_TYPE(AvmClass,
               AvmType,
               {
                   [FnEntryToString] = (AvmCallback)AvmClassToString,
               });

bool AvmEnumIsDefined(const AvmEnum* self, _long value)
{
    pre
    {
        assert(self != NULL);
    }

    for (uint i = 0; i < self->_private.constantCount; i++)
    {
        if (self->_private.constants[i]._value == value)
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
        if (self->_private.constants[i]._value == 0 &&
            self->_private.constants[i]._name == NULL)
        {
            break;
        }

        if (self->_private.constants[i]._value == value)
        {
            return self->_private.constants[i]._name;
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
        if (self->_private.constants[i]._value == 0 &&
            self->_private.constants[i]._name == NULL)
        {
            break;
        }

        if (strcmp(self->_private.constants[i]._name, name) == 0)
        {
            return self->_private.constants[i]._value;
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

    return AvmStringFormat("enum %s (%u bytes)",
                           AvmTypeGetName(&self->__base),
                           AvmTypeGetSize(&self->__base));
}

AVM_CLASS_TYPE(AvmEnum,
               AvmType,
               {
                   [FnEntryToString] = (AvmCallback)AvmEnumToString,
               });

uint AvmMemberGetOffset(const AvmMember* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_private.offset;
}

static AvmString AvmMemberToString(const AvmMember* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat("member %s (%u bytes offset)",
                           AvmTypeGetName(&self->__base),
                           AvmMemberGetOffset(self));
}

AVM_CLASS_TYPE(AvmMember,
               AvmType,
               {
                   [FnEntryToString] = (AvmCallback)AvmMemberToString,
               });

const AvmType* AvmFunctionGetReturnType(const AvmFunction* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_private.returnType;
}

const AvmType** AvmFunctionGetParams(const AvmFunction* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_private.paramTypes;
}

uint AvmFunctionGetParamCount(const AvmFunction* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_private.paramCount;
}

static AvmString AvmFunctionToString(const AvmFunction* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmString s = AvmStringNew(self->_private.paramCount * 4);
    AvmStringPushStr(&s, "function ");
    AvmStringPushStr(&s, AvmTypeGetName(&self->__base));
    AvmStringPushStr(&s, " (");
    AvmStringPushUint(&s, self->_private.paramCount, AvmNumericBaseDecimal);
    if (self->_private.paramCount == 1)
    {
        AvmStringPushStr(&s, " parameter)");
    }
    else
    {
        AvmStringPushStr(&s, " parameters)");
    }
    return s;
}

AVM_CLASS_TYPE(AvmFunction,
               AvmType,
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

    const size_t size = AvmTypeGetSize((const AvmType*)AvmObjectGetType(self));
    return memcmp(self, other, size) == 0;
}

static object objectClone(object self)
{
    pre
    {
        assert(self != NULL);
    }

    const AvmClass* type = AvmObjectGetType(self);
    object o = AvmObjectNew(type);
    AvmCopy(self, AvmTypeGetSize((const AvmType*)type), (byte*)o);
    return o;
}

static AvmString objectToString(object self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(
        AVM_OBJECT_FMT_STR,
        AvmTypeGetName((const AvmType*)AvmObjectGetType(self)),
        self);
}

// TODO: Maybe all these should not inherit from object?
AVM_CLASS_TYPE(object,
               object,
               {
                   [FnEntryFinalize] = (AvmCallback)objectFinalize,
                   [FnEntryEquals] = (AvmCallback)objectEquals,
                   [FnEntryClone] = (AvmCallback)objectClone,
                   [FnEntryToString] = (AvmCallback)objectToString,
               });

AVM_CLASS_TYPE(_long, object, AVM_VTABLE_DEFAULT);
AVM_CLASS_TYPE(ulong, object, AVM_VTABLE_DEFAULT);
AVM_CLASS_TYPE(int, object, AVM_VTABLE_DEFAULT);
AVM_CLASS_TYPE(uint, object, AVM_VTABLE_DEFAULT);
AVM_CLASS_TYPE(short, object, AVM_VTABLE_DEFAULT);
AVM_CLASS_TYPE(ushort, object, AVM_VTABLE_DEFAULT);
AVM_CLASS_TYPE(char, object, AVM_VTABLE_DEFAULT);
AVM_CLASS_TYPE(byte, object, AVM_VTABLE_DEFAULT);
AVM_CLASS_TYPE(bool, object, AVM_VTABLE_DEFAULT);
AVM_CLASS_TYPE(float, object, AVM_VTABLE_DEFAULT);
AVM_CLASS_TYPE(double, object, AVM_VTABLE_DEFAULT);
AVM_CLASS_TYPE(str, object, AVM_VTABLE_DEFAULT);

// TODO
// Because we can't sizeof(void) we have to do this manually.
// static AvmCallback AVM_VT_NAME(void)[] = AVM_VTABLE_DEFAULT;

// const AvmType AVM_TI_NAME(void) = {
//     .__type = typeid(AvmType),
//     ._vPtr = AVM_VT_NAME(void),
//     ._name = "void",
//     .__baseType = typeid(object),
//     ._vSize = sizeof(AVM_VT_NAME(void)),
//     ._size = 0,
// };
