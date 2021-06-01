#ifndef AVIUM_DLFCN_H
#define AVIUM_DLFCN_H

#include "avium/core.h"

#ifdef AVM_WIN32
#define RTLD_LAZY         0
#define RTLD_NOW          0
#define RTLD_BINDING_MASK 0
#define RTLD_NOLOAD       0
#define RTLD_DEEPBIND     0
#define RTLD_GLOBAL       0
#define RTLD_LOCAL        0
#define RTLD_NODELETE     0
extern void* dlopen(const char* filename, int mode);
extern int dlclose(void* handle);
extern void* dlsym(void* handle, const char* name);
extern char* dlerror(void);

typedef struct
{
    const char* dli_fname;
    void* dli_fbase;
    const char* dli_sname;
    void* dli_saddr;
} Dl_info;

extern int dladdr(const void* address, Dl_info* info);
#else
#include <dlfcn.h>
#endif

#endif // AVIUM_DLFCN_H
