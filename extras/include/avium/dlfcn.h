#ifndef AVIUM_DLFCN_H
#define AVIUM_DLFCN_H

#include "avium/types.h"

extern void* AvmDlopen(str filename, int mode);
extern int AvmDlclose(void* handle);
extern void* AvmDlsym(void* handle, str name);
extern char* AvmDlerror(void);

#ifdef AVM_WIN32
#    define RTLD_LAZY         0
#    define RTLD_NOW          0
#    define RTLD_BINDING_MASK 0
#    define RTLD_NOLOAD       0
#    define RTLD_DEEPBIND     0
#    define RTLD_GLOBAL       0
#    define RTLD_LOCAL        0
#    define RTLD_NODELETE     0
#else
#    include <dlfcn.h>
#endif

#endif  // AVIUM_DLFCN_H
