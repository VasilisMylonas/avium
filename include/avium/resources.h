#include "avium/prologue.h"

#define AVM_FLOAT_BUFFER_SIZE 128

static const str SelfNullMsg = "Parameter `self` was `NULL`.";
static const str ContentsNullMsg = "Parameter `contents` was `NULL`.";
static const str OtherNullMsg = "Parameter `other` was `NULL`.";
static const str SubstringNullMsg = "Parameter `substring` was `NULL`.";
static const str FormatNullMsg = "Parameter `format` was `NULL`.";
static const str ArgsNullMsg = "Parameter `args` was `NULL`.";
static const str HandleNullMsg = "Parameter `handle` was `NULL`.";
static const str SourceNullMsg = "Parameter `source` was `NULL`.";
static const str DestinationNullMsg = "Parameter `destination` was `NULL`.";
static const str BaseOutOfRangeMsg = "Parameter `base` was out of range.";
static const str StringNullMsg = "Parameter `string` was `NULL`.";
static const str BufferNullMsg = "Parameter `buffer` was `NULL`.";
static const str VirtualFuncTrapTriggered = "Virtual function trap triggered.";

static const str IndexOutOfRange = "The specified index was out of range.";
static const str VersionFormat = "%i.%i.%i-%c";
