#ifndef AVIUM_PRIVATE_CONSTANTS_H
#define AVIUM_PRIVATE_CONSTANTS_H

#include "avium/config.h"

#define RAW_MEMORY        ((void*)0)
#define AVM_OBJECT        ((void*)1)
#define ALREADY_FINALIZED ((void*)2)

#define AVM_STRING_DEFAULT_CAPACITY 8
#define BACKTRACE_MAX_SYMBOLS       128
#define BACKTRACE_LINE_SIZE         32
#define AVM_FLOAT_BUFFER_SIZE       128
#define READ_LINE_CAPACITY          32

#define CAST_FAIL_STR "Tried to cast object [%x] of type %T to type %s."
#define LOCATION_STR  "%s:%u"
#define VERSION_STR   "%u.%u.%u"
#define RUNTIME_STR   "%s v%v"
#define OBJECT_STR    "%s [%x]"
#define UNHANDLED_THROW_STR                                                    \
    "Uncaught thrown object of type %T: %v\nThrown from %v\n"
#define AVM_RUNTIME_NAME            "Avium x86_64 Runtime Environment"
#define BACKTRACE_SEPARATOR         '@'
#define BACKTRACE_NOT_AVAILABLE_MSG "Backtrace not available."
#define LONG_MIN_STR                "-9223372036854775808"
#define TYPE_SYMBOL_STR             "_TI_%s"
#define MODULE_STR                  "Module \"%s\" loaded at %x"

#if defined AVM_WIN32
#define LIB_STR "%s.dll"
#elif defined AVM_DARWIN
#define LIB_STR "lib%s.dylib"
#else
#define LIB_STR "lib%s.so"
#endif

#ifdef AVM_WIN32
static const str HomeEnvVar = "%HOMEDRIVE%%HOMEPATH%";
#else
static const str HomeEnvVar = "HOME";
#endif

static const str ArgError = "Received an invalid argument.";
static const str RangeError = "The provided index was out of range.";
static const str MemError = "The runtime run out of memory.";
static const str InvalidOpError = "The call was invalid for the current state.";
static const str IOError = "An IO error occurred.";
static const str SysError = "An unknown system error occurred.";
static const str NotFoundError = "A required resource was unavailable.";
static const str ReadError = "Could not perform the read operation.";
static const str WriteError = "Could not perform the write operation.";
static const str InternalError = "An internal error occurred.";
static const str EnumConstantNotPresentError =
    "The specified constant was not present in the enum.";
static const str VirtualFuncError = "Virtual function entry not present.";
static const str HomeDirNotDeterminedError =
    "Could not determine the home directory.";
static const str MarshallingError =
    "Marshalling of non primitive types is not supported.";
static const str SymbolNotPresentError =
    "The requested symbol was not present.";
static const str TypeNotPresentError = "The requested type was not present.";

#endif // AVIUM_PRIVATE_CONSTANTS_H
