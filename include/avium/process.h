#ifndef AVIUM_PROCESS_H
#define AVIUM_PROCESS_H

#include "avium/core.h"

AVM_CLASS(AvmProcess, object, { void* _state; });

AVMAPI void AvmProcessExit(AvmExitCode code);
AVMAPI void AvmProcessFastExit(AvmExitCode code);
AVMAPI void AvmProcessTerminate(AvmProcess* self);

AVMAPI AvmProcess AvmProcessNew(str path, uint length, str args[]);
AVMAPI AvmExitCode AvmProcessWait(const AvmProcess* self);

#endif // AVIUM_PROCESS_H
