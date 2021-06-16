#include "avium/typeinfo.h"

#include "avium/core.h"
#include "avium/string.h"

#include <stdlib.h>
#include <string.h>

#define _ AvmRuntimeGetResource

//
// AvmType implementation.
//

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
                   [AvmEntryToString] = (AvmCallback)AvmTypeToString,
               });

//
// AvmInterface implementation.
//

static AvmString AvmInterfaceToString(const AvmInterface* self)
{
    return AvmStringFormat("interface %s (%u methods)",
                           AvmTypeGetName(baseof(self)),
                           baseof(self)->_private.vCount);
}

AVM_CLASS_TYPE(AvmInterface,
               AvmType,
               {
                   [AvmEntryToString] = (AvmCallback)AvmInterfaceToString,
               });

//
// AvmClass implementation.
//

static const AvmInterface* AvmClassGetInterfaceImpl(const AvmClass* self,
                                                    str name)
{
    pre
    {
        assert(self != NULL);
        assert(name != NULL);
    }

    const uint count = self->_private.interfaceCount;
    const AvmInterface** interfaces = self->_private.interfaces;

    // This is a special case. See AVM_INTERFACES_DEFAULT.
    if (count == 1 && interfaces[0] == NULL)
    {
        return AvmClassGetInterfaceImpl(AvmClassGetBase(self), name);
    }

    for (uint i = 0; i < count; i++)
    {
        if (strcmp(interfaces[i]->__base._private.name, name) == 0)
        {
            return interfaces[i];
        }
    }

    // We reached the bottom type, no such interface was found.
    if (self == typeid(object))
    {
        return NULL;
    }

    // It is easier if we use recursion.
    return AvmClassGetInterfaceImpl(AvmClassGetBase(self), name);
}

const AvmInterface* AvmClassGetInterface(const AvmClass* self, str name)
{
    pre
    {
        assert(self != NULL);
        assert(name != NULL);
    }

    const AvmInterface* interface = AvmClassGetInterfaceImpl(self, name);

    if (interface == NULL)
    {
        throw(AvmErrorNew(_(AvmMissingInterfaceErrorMsg)));
    }
    else
    {
        return interface;
    }
}

bool AvmClassImplements(const AvmClass* self, str name)
{
    pre
    {
        assert(self != NULL);
        assert(name != NULL);
    }

    return AvmClassGetInterfaceImpl(self, name) != NULL;
}

AvmCallback AvmClassGetCallback(const AvmClass* self, uint index)
{
    pre
    {
        assert(self != NULL);
    }

    const uint length = baseof(self)->_private.vCount;

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

    const uint count = self->_private.memberCount;

    // This is a special case. See AVM_MEMBERS_DEFAULT.
    if (count == 1 && self->_private.members[0].__type == NULL)
    {
        throw(AvmErrorNew(_(AvmMissingMemberErrorMsg)));
    }

    if (index < count)
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

    const uint count = self->_private.memberCount;

    // This is a special case. See AVM_MEMBERS_DEFAULT.
    if (count == 1 && self->_private.members[0].__type == NULL)
    {
        throw(AvmErrorNew(_(AvmMissingMemberErrorMsg)));
    }

    for (uint i = 0; i < count; i++)
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

static AvmString AvmClassToString(const AvmClass* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat("class %s (%u bytes)",
                           AvmTypeGetName(&self->__base),
                           AvmTypeGetSize(&self->__base));
}

AVM_CLASS_TYPE(AvmClass,
               AvmType,
               {
                   [AvmEntryToString] = (AvmCallback)AvmClassToString,
               });

//
// AvmMember implementation.
//

uint AvmMemberGetOffset(const AvmMember* self)
{
    pre
    {
        assert(self != NULL);
    }

    return baseof(self)->_private.offset;
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
                   [AvmEntryToString] = (AvmCallback)AvmMemberToString,
               });

//
// AvmEnum implementation.
//

bool AvmEnumIsDefined(const AvmEnum* self, _long value)
{
    pre
    {
        assert(self != NULL);
    }

    for (uint i = 0; i < baseof(self)->_private.constantCount; i++)
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
                   [AvmEntryToString] = (AvmCallback)AvmEnumToString,
               });

//
// AvmFunction implementation.
//

const AvmType* AvmFunctionGetReturnType(const AvmFunction* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_private.returnType;
}

const AvmType* AvmFunctionGetParam(const AvmFunction* self, uint index)
{
    pre
    {
        assert(self != NULL);
        assert(index < AvmFunctionGetParamCount(self));
    }

    return self->_private.paramTypes[index];
}

uint AvmFunctionGetParamCount(const AvmFunction* self)
{
    pre
    {
        assert(self != NULL);
    }

    return baseof(self)->_private.paramCount;
}

static AvmString AvmFunctionToString(const AvmFunction* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmString s = AvmStringNew(baseof(self)->_private.paramCount * 4);
    AvmStringPushStr(&s, "function ");
    AvmStringPushStr(&s, AvmTypeGetName(&self->__base));
    AvmStringPushStr(&s, " (");
    AvmStringPushUint(
        &s, baseof(self)->_private.paramCount, AvmNumericBaseDecimal);
    if (baseof(self)->_private.paramCount == 1)
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
                   [AvmEntryToString] = (AvmCallback)AvmFunctionToString,
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

AVM_IMPLEMENT(AvmEquatable,
              object,
              {
                  [AvmEquatableEquals] = (AvmCallback)objectEquals,
              });

AVM_IMPLEMENT(AvmCloneable,
              object,
              {
                  [AvmCloneableClone] = (AvmCallback)objectClone,
              });

AVM_INTERFACES(object,
               {
                   interfaceid(AvmEquatable, object),
                   interfaceid(AvmCloneable, object),
               });

AVM_MEMBERS(object, AVM_MEMBERS_DEFAULT);

// TODO: Maybe all these should not inherit from object?
AVM_CLASS_TYPE_EX(object,
                  object,
                  {
                      [AvmEntryFinalize] = (AvmCallback)objectFinalize,
                      [AvmEntryToString] = (AvmCallback)objectToString,
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

// Because we can't sizeof(void) we have to do this manually.
static const AvmCallback _AVM_VTABLE_BLOCK_NAME(void)[] = AVM_VTABLE_DEFAULT;
const AvmClass _AVM_METADATA_BLOCK_NAME(void) = {
    .__base.__type = typeid(AvmClass),
    .__base._private =
        {
            .name = "void",
            .size = 0,
            .vCount = sizeof(_AVM_VTABLE_BLOCK_NAME(void)) /
                      sizeof(AvmCallback),
        },
    ._private =
        {
            .base = typeid(object),
            .vPtr = _AVM_VTABLE_BLOCK_NAME(void),
        },
};

AVM_CLASS_TYPE(AvmInterfaceObject, object, AVM_VTABLE_DEFAULT);

//
// Misc.
//

object __AvmRuntimeCast(object value, const AvmClass* type)
{
    pre
    {
        assert(value != NULL);
        assert(type != NULL);
    }

    const AvmClass* objType = AvmObjectGetType(value);

    if (type == objType || AvmClassInheritsFrom(objType, type))
    {
        return value;
    }

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
