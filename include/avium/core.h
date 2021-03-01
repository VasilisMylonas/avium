#ifndef AVIUM_CORE_H
#define AVIUM_CORE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "avium/config.h"

typedef char char8_t;              /// 8 bit character type.
typedef void* object_t;            /// Represents an unknown class type.
typedef void (*function_t)(void);  /// Represents an unknown function type.

#ifdef AVM_MSVC
#    define never __declspec(noreturn) void
#else
#    define never _Noreturn void
#endif  // AVM_MSVC

typedef enum {
    EK_ARGUMENT,
    EK_OUT_OF_RANGE,
    EK_OUT_OF_MEMORY,
    EK_INVALID_OPERATION,
    EK_IO,
    EK_SYSTEM,
    EK_NOT_FOUND,
} AvmErrorKind;

typedef union {
    AvmErrorKind kind;
    object_t value;
} AvmResult;

typedef struct Type* Type;

static inline AvmResult AvmSuccess(object_t value) {
    return (AvmResult){.value = value};
}

static inline AvmResult AvmFailure(AvmErrorKind kind) {
    return (AvmResult){.kind = kind};
}

static inline bool AvmResultIsFailure(AvmResult self) {
    return ((uintptr_t)self.value) < 512;
}

typedef struct AvmString* AvmString;

AVMAPI Type AvmObjectType(object_t self);
AVMAPI const char8_t* AvmObjectName(object_t self);
AVMAPI size_t AvmObjectSize(object_t self);
AVMAPI bool AvmObjectEq(object_t lhs, object_t rhs);
AVMAPI void AvmDestroy(object_t object);
AVMAPI object_t AvmClone(object_t object);
AVMAPI AvmString AvmToString(object_t object);
AVMAPI size_t AvmGetLength(object_t object);
AVMAPI size_t AvmGetCapacity(object_t object);

AVMAPI never AvmPanic(const char8_t* message, const char8_t* function,
                      const char8_t* file, uint32_t line);

#define panic(message) AvmPanic(message, __func__, __FILE__, __LINE__)

#define AVM_CONCAT_(x, y) x##y
#define AVM_CONCAT(x, y)  AVM_CONCAT_(x, y)

#define defer(x)                                           \
    for (size_t AVM_CONCAT(avmDeferCounter, __LINE__) = 0; \
         AVM_CONCAT(avmDeferCounter, __LINE__) < 1;        \
         AVM_CONCAT(avmDeferCounter, __LINE__)++, AvmDestroy(x))

#endif  // AVIUM_CORE_H
