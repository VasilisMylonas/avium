#ifndef AVIUM_RESOURCES_H
#define AVIUM_RESOURCES_H

#include "avium/types.h"

#define AVM_FLOAT_BUFFER_SIZE 128
#define BACKTRACE_MAX_SYMBOLS 128
#define READ_LINE_CAPACITY    32

AVMAPI const str LongMinRepr;
AVMAPI const str SelfNullMsg;
AVMAPI const str ContentsNullMsg;
AVMAPI const str OtherNullMsg;
AVMAPI const str SubstringNullMsg;
AVMAPI const str FormatNullMsg;
AVMAPI const str ArgsNullMsg;
AVMAPI const str HandleNullMsg;
AVMAPI const str SourceNullMsg;
AVMAPI const str DestinationNullMsg;
AVMAPI const str NumericBaseOutOfRangeMsg;
AVMAPI const str StringNullMsg;
AVMAPI const str BufferNullMsg;
AVMAPI const str IndexOutOfRange;
AVMAPI const str FunctionNullMsg;
AVMAPI const str InvalidValueCastMsg;
AVMAPI const str InvalidValueKindMsg;
AVMAPI const str NoBacktraceMsg;
AVMAPI const str InvalidPtrDerefMsg;
AVMAPI const str IllegalInstructionMsg;
AVMAPI const str ArithmeticExceptionMsg;
AVMAPI const str InvalidOriginMsg;
AVMAPI const str PathNullMsg;

#endif  // AVIUM_RESOURCES_H
