#ifndef AVIUM_RESOURCES_H
#define AVIUM_RESOURCES_H

#include "avium/types.h"

#define AVM_FLOAT_BUFFER_SIZE 128
#define BACKTRACE_MAX_SYMBOLS 128
#define READ_LINE_CAPACITY    32

static const str LongMinRepr = "-9223372036854775808";
static const str SelfNullMsg = "Parameter `self` was `NULL`.";
static const str ContentsNullMsg = "Parameter `contents` was `NULL`.";
static const str OtherNullMsg = "Parameter `other` was `NULL`.";
static const str SubstringNullMsg = "Parameter `substring` was `NULL`.";
static const str FormatNullMsg = "Parameter `format` was `NULL`.";
static const str ArgsNullMsg = "Parameter `args` was `NULL`.";
static const str HandleNullMsg = "Parameter `handle` was `NULL`.";
static const str SourceNullMsg = "Parameter `source` was `NULL`.";
static const str DestinationNullMsg = "Parameter `destination` was `NULL`.";
static const str NumericBaseOutOfRangeMsg =
    "Parameter `numericBase` was out of range.";
static const str StringNullMsg = "Parameter `string` was `NULL`.";
static const str BufferNullMsg = "Parameter `buffer` was `NULL`.";
static const str IndexOutOfRange = "The specified index was out of range.";
static const str FunctionNullMsg = "Parameter `function` was `NULL`.";
static const str InvalidValueCastMsg = "Invalid AvmValue cast.";
static const str InvalidValueKindMsg = "Invalid AvmValue kind.";
static const str NoBacktraceMsg = "No backtrace is available.";
static const str InvalidPtrDerefMsg = "Invalid pointer dereference.";
static const str IllegalInstructionMsg = "Illegal instruction.";
static const str ArithmeticExceptionMsg = "Arithmetic exception.";
static const str InvalidOriginMsg = "Parameter `origin` was invalid.";
static const str PathNullMsg = "Parameter `path` was `NULL`.";

#endif  // AVIUM_RESOURCES_H
