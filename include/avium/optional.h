#ifndef AVIUM_OPTIONAL_H
#define AVIUM_OPTIONAL_H

#include "avium/runtime.h"

#define AvmOptional(T) AVM_GENERIC(AvmOptional, T)
#define AvmSome(T)     AVM_GENERIC(AvmSome, T)
#define AvmNone(T)     AVM_GENERIC(AvmNone, T)
#define AvmHasValue(T) AVM_GENERIC(AvmHasValue, T)
#define AvmGetValue(T) AVM_GENERIC(AvmGetValue, T)

#define AVM_OPTIONAL_TYPE(T)                                              \
    AVM_INLINE_CLASS(AVM_GENERIC(AvmOptional, T), object, {               \
        T _value;                                                         \
        bool _hasValue;                                                   \
    });                                                                   \
                                                                          \
    AVM_INLINE_TYPE(AVM_GENERIC(AvmOptional, T), {[FnEntryDtor] = NULL}); \
                                                                          \
    static inline AvmOptional(T) AvmSome(T)(T value) {                    \
        return (AvmOptional(T)){                                          \
            ._type = typeid(AVM_GENERIC(AvmOptional, T)),                 \
            ._hasValue = true,                                            \
            ._value = value,                                              \
        };                                                                \
    }                                                                     \
                                                                          \
    static inline AvmOptional(T) AvmNone(T)(void) {                       \
        return (AvmOptional(T)){                                          \
            ._type = typeid(AVM_GENERIC(AvmOptional, T)),                 \
            ._hasValue = false,                                           \
        };                                                                \
    }                                                                     \
                                                                          \
    static inline bool AvmHasValue(T)(AvmOptional(T) * self) {            \
        return self->_hasValue;                                           \
    }                                                                     \
                                                                          \
    static inline T AvmGetValue(T)(AvmOptional(T) * self) {               \
        if (self->_hasValue) {                                            \
            return self->_value;                                          \
        }                                                                 \
                                                                          \
        AvmPanic("Tried to unwrap an empty optional.");                   \
    }

AVM_OPTIONAL_TYPE(char)
AVM_OPTIONAL_TYPE(byte)
AVM_OPTIONAL_TYPE(short)
AVM_OPTIONAL_TYPE(ushort)
AVM_OPTIONAL_TYPE(int)
AVM_OPTIONAL_TYPE(uint)
AVM_OPTIONAL_TYPE(_long)
AVM_OPTIONAL_TYPE(ulong)
AVM_OPTIONAL_TYPE(bool)
AVM_OPTIONAL_TYPE(str)
AVM_OPTIONAL_TYPE(object)
AVM_OPTIONAL_TYPE(size_t)
AVM_OPTIONAL_TYPE(AvmString)
AVM_OPTIONAL_TYPE(AvmFunction)

#endif  // AVIUM_OPTIONAL_H
