#ifndef AVIUM_RUNTIME_H
#define AVIUM_RUNTIME_H

#include "avium/types.h"

AVMAPI void AvmRuntimeEnableExceptions(void);
AVMAPI void AvmRuntimeDisableExceptions(void);
AVMAPI never AvmRuntimeVirtualFunctionTrap(void);

#endif  // AVIUM_RUNTIME_H
