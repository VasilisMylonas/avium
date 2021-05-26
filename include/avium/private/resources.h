#ifndef AVIUM_RESOURCES_H
#define AVIUM_RESOURCES_H

#include "avium/types.h"

#define VIRTUAL_CALL_(TReturn, E, ...)                                         \
    AvmFunction __virtualFunc =                                                \
        AvmTypeGetFunction(AvmObjectGetType((object)self), E);                 \
    return ((TReturn(*)())__virtualFunc)(__VA_ARGS__);

#ifdef AVM_GNU
#define VIRTUAL_CALL(TReturn, E, ...)                                          \
    _Pragma("GCC diagnostic push")                                             \
        _Pragma("GCC diagnostic ignored \"-Wpedantic\"")                       \
            VIRTUAL_CALL_(TReturn, E, __VA_ARGS__);                            \
    _Pragma("GCC diagnostic pop")
#else
#define VIRTUAL_CALL(TReturn, E, ...) VIRTUAL_CALL_(TReturn, E, __VA_ARGS__)
#endif

#define BACKTRACE_MAX_SYMBOLS 128
#define AVM_FLOAT_BUFFER_SIZE 128
#define READ_LINE_CAPACITY    32

static const str LongMinRepr = "-9223372036854775808";
static const str NumericBaseOutOfRangeMsg =
    "Parameter `numericBase` was out of range.";
static const str InvalidOriginMsg = "Parameter `origin` was invalid.";
static const str InvalidAccessMsg = "Parameter `access` was invalid.";
static const str InvalidPtrDerefMsg = "Invalid pointer dereference.";
static const str IllegalInstructionMsg = "Illegal instruction.";
static const str ArithmeticExceptionMsg = "Arithmetic exception.";
static const str NoBacktraceMsg = "No backtrace is available.";

#endif // AVIUM_RESOURCES_H
