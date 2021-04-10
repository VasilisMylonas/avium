#ifndef AVIUM_ERROR_H
#define AVIUM_ERROR_H

#include "avium/types.h"

typedef struct AvmError AvmError;
struct AvmError {
    AvmType* _type;
};

AVMAPI AvmError* AvmErrorFromErrno(void);

#endif  // AVIUM_ERROR_H
