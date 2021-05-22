#ifndef AVIUM_RESOURCES_H
#define AVIUM_RESOURCES_H

#include "avium/types.h"

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

#ifdef AVM_WIN32
static const str HomeEnvVar = "%HOMEDRIVE%%HOMEPATH%"
#else
static const str HomeEnvVar = "HOME";
#endif

#endif // AVIUM_RESOURCES_H
