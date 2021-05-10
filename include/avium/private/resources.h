#ifndef AVIUM_RESOURCES_H
#define AVIUM_RESOURCES_H

#include "avium/types.h"

#define AVM_FLOAT_BUFFER_SIZE 128
#define BACKTRACE_MAX_SYMBOLS 128
#define READ_LINE_CAPACITY    32

#define NULL_PARAM_STR(name)  "Parameter `" name "` was `NULL`."
#define RANGE_PARAM_STR(name) "Parameter `" name "` was out of range."

static const str LongMinRepr = "-9223372036854775808";
static const str SelfNullMsg = NULL_PARAM_STR("self");
static const str ContentsNullMsg = NULL_PARAM_STR("contents");
static const str OtherNullMsg = NULL_PARAM_STR("other");
static const str SubstringNullMsg = NULL_PARAM_STR("substring");
static const str FormatNullMsg = NULL_PARAM_STR("format");
static const str ArgsNullMsg = NULL_PARAM_STR("args");
static const str HandleNullMsg = NULL_PARAM_STR("handle");
static const str SourceNullMsg = NULL_PARAM_STR("source");
static const str DestinationNullMsg = NULL_PARAM_STR("destination");
static const str NumericBaseOutOfRangeMsg = RANGE_PARAM_STR("numericBase");
static const str StringNullMsg = NULL_PARAM_STR("string");
static const str BufferNullMsg = NULL_PARAM_STR("buffer");
static const str IndexOutOfRange = "The specified index was out of range.";
static const str FunctionNullMsg = NULL_PARAM_STR("function");
static const str InvalidValueCastMsg = "Invalid AvmValue cast.";
static const str InvalidValueKindMsg = "Invalid AvmValue kind.";
static const str NoBacktraceMsg = "No backtrace is available.";
static const str InvalidPtrDerefMsg = "Invalid pointer dereference.";
static const str IllegalInstructionMsg = "Illegal instruction.";
static const str ArithmeticExceptionMsg = "Arithmetic exception.";
static const str InvalidOriginMsg = "Parameter `origin` was invalid.";
static const str PathNullMsg = NULL_PARAM_STR("path");
static const str InvalidAccessMsg = "Parameter `access` was invalid.";
static const str InternalErrorMsg = "Internal error.";
static const str NotImplementedMsg = "Not implemented.";
static const str NameNullMsg = NULL_PARAM_STR("name");
static const str MissingSymbolMsg = "The requested symbol was missing.";

#endif // AVIUM_RESOURCES_H
