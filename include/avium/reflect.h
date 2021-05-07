#ifndef AVIUM_REFLECT_H
#define AVIUM_REFLECT_H

#include "avium/config.h"

#ifdef AVM_USE_REFLECT

#    include "avium/types.h"
#    include "avium/array-list.h"

typedef enum {
    SymbolTypeUnknown = 0,
    SymbolTypeVariable,
    SymbolTypeFunction,
    SymbolTypeType,
} AvmSymbolType;

AVM_CLASS(AvmModule, object, {
    void* _handle;
    str _name;
    AvmArrayList(AvmString) _symbols;
});

AVMAPI AvmModule AvmModuleLoad(str path);
AVMAPI AvmModule* AvmModuleGetCurrent(void);

AVMAPI str AvmModuleGetName(AvmModule* self);
AVMAPI size_t AvmModuleGetSymbolCount(AvmModule* self);
AVMAPI bool AvmModuleHasSymbol(AvmModule* self, str name);
AVMAPI AvmSymbolType AvmModuleGetSymbolType(AvmModule* self, str name);
AVMAPI AvmType* AvmModuleGetType(AvmModule* self, str name);
AVMAPI AvmFunction AvmModuleGetFunction(AvmModule* self, str name);
AVMAPI void* AvmModuleGetVariable(AvmModule* self, str name);

AVMAPI object AvmReflectConstructType(AvmType* type);

AVM_CLASS(AvmEnum, object, {
    str _name;
    size_t _size;
    struct {
        str _name;
        long _value;
    } _members[AVM_MAX_ENUM_MEMBERS];
});

AVMAPI str AvmEnumGetName(AvmEnum* self);
AVMAPI size_t AvmEnumGetSize(AvmEnum* self);
AVMAPI bool AvmEnumIsDefined(AvmEnum* self, long value);
AVMAPI str AvmEnumGetNameOf(AvmEnum* self, long value);
AVMAPI long AvmEnumGetValueOf(AvmEnum* self, str name);

#    define AVM_ENUM_MEMBER(V) \
        { #V, V }

#    define AVM_ENUM(T, ...) AVM_ENUM_(T, __VA_ARGS__)

#    define enumid(T) (&AVM_EI_NAME(T))

#    define AVM_ENUM_(T, ...)         \
        AvmEnum AVM_EI_NAME(T) = {    \
            ._type = typeid(AvmEnum), \
            ._name = #T,              \
            ._size = sizeof(T),       \
            ._members = __VA_ARGS__,  \
        }

AVMAPI AvmArrayList(AvmString)
    __AvmGetSymbolList(str path, str nmCommandTemplate);

#endif  // AVM_USE_REFLECT

#endif  // AVIUM_REFLECT_H
