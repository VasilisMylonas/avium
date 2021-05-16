#ifndef AVIUM_REFLECT_H
#define AVIUM_REFLECT_H

#include "avium/core.h"

AVMAPI const AvmType* AvmReflectLoadType(str name);

#ifdef AVM_LINUX
AVMAPI void AvmReflectPrintLoadedLibs();
#endif

#endif // AVIUM_REFLECT_H
