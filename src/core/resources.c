#include "avium/resources.h"

#define NULL_PARAM_STR(name)  "Parameter `" name "` was `NULL`."
#define RANGE_PARAM_STR(name) "Parameter `" name "` was out of range."

const str LongMinRepr = "-9223372036854775808";
const str SelfNullMsg = NULL_PARAM_STR("self");
const str ContentsNullMsg = NULL_PARAM_STR("contents");
const str OtherNullMsg = NULL_PARAM_STR("other");
const str SubstringNullMsg = NULL_PARAM_STR("substring");
const str FormatNullMsg = NULL_PARAM_STR("format");
const str ArgsNullMsg = NULL_PARAM_STR("args");
const str HandleNullMsg = NULL_PARAM_STR("handle");
const str SourceNullMsg = NULL_PARAM_STR("source");
const str DestinationNullMsg = NULL_PARAM_STR("destination");
const str NumericBaseOutOfRangeMsg = RANGE_PARAM_STR("numericBase");
const str StringNullMsg = NULL_PARAM_STR("string");
const str BufferNullMsg = NULL_PARAM_STR("buffer");
const str IndexOutOfRange = "The specified index was out of range.";
const str FunctionNullMsg = NULL_PARAM_STR("function");
const str InvalidValueCastMsg = "Invalid AvmValue cast.";
const str InvalidValueKindMsg = "Invalid AvmValue kind.";
const str NoBacktraceMsg = "No backtrace is available.";
const str InvalidPtrDerefMsg = "Invalid pointer dereference.";
const str IllegalInstructionMsg = "Illegal instruction.";
const str ArithmeticExceptionMsg = "Arithmetic exception.";
const str InvalidOriginMsg = "Parameter `origin` was invalid.";
const str PathNullMsg = NULL_PARAM_STR("path");
