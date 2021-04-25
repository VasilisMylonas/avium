#ifndef AVIUM_RESOURCES_H
#define AVIUM_RESOURCES_H

#include "avium/types.h"

#define AVM_FLOAT_BUFFER_SIZE 128
#define BACKTRACE_MAX_SYMBOLS 128
#define READ_LINE_CAPACITY    32

extern const str LongMinRepr;
extern const str SelfNullMsg;
extern const str ContentsNullMsg;
extern const str OtherNullMsg;
extern const str SubstringNullMsg;
extern const str FormatNullMsg;
extern const str ArgsNullMsg;
extern const str HandleNullMsg;
extern const str SourceNullMsg;
extern const str DestinationNullMsg;
extern const str NumericBaseOutOfRangeMsg;
extern const str StringNullMsg;
extern const str BufferNullMsg;
extern const str IndexOutOfRange;
extern const str FunctionNullMsg;
extern const str InvalidValueCastMsg;
extern const str InvalidValueKindMsg;
extern const str NoBacktraceMsg;
extern const str InvalidPtrDerefMsg;
extern const str IllegalInstructionMsg;
extern const str ArithmeticExceptionMsg;
extern const str InvalidOriginMsg;
extern const str PathNullMsg;

#endif  // AVIUM_RESOURCES_H
