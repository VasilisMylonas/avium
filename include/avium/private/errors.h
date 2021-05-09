#include "avium/types.h"

static const str ArgError = "Received an invalid argument.";
static const str RangeError = "The provided index was out of range.";
static const str MemError = "There was not enough memory to handle the request.";
static const str InvalidOpError = "The call was invalid for the current state.";
static const str IOError = "An IO error occurred.";
static const str SysError = "An unknown system error occurred.";
static const str NotFoundError = "A required resource was unavailable.";
static const str ReadError = "Could not perform the read operation.";
static const str WriteError = "Could not perform the write operation.";
