#include "avium/dlfcn.h"

#ifdef AVM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// This should come after windows.h
#include <dbghelp.h>

static __declspec(thread) DWORD last_dl_error;

void* dlopen(const char* filename, int mode)
{
    (void)mode;

    if (filename == NULL)
    {
        return GetModuleHandleA(NULL);
    }

    void* handle = LoadLibraryA(filename);
    last_dl_error = GetLastError();
    return handle;
}

int dlclose(void* handle)
{
    BOOL b = FreeLibrary(handle);
    last_dl_error = GetLastError();
    return !b;
}

void* dlsym(void* handle, const char* name)
{
    FARPROC proc = GetProcAddress(handle, name);
    last_dl_error = GetLastError();
    return *(void**)&proc;
}

char* dlerror(void)
{
    char* message = NULL;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL,
                  last_dl_error,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPSTR)&message,
                  0,
                  NULL);

    return message;
}

int dladdr(const void* address, Dl_info* info)
{
    DWORD64 displacement = 0;
    HANDLE process = GetCurrentProcess();
    if (!SymInitialize(process, NULL, FALSE))
    {
        return 0;
    }

    PSYMBOL_INFO i = AvmAlloc(sizeof(SYMBOL_INFO) + MAX_SYM_NAME);
    i->SizeOfStruct = sizeof(SYMBOL_INFO);
    i->MaxNameLen = MAX_SYM_NAME;

    // TODO: Not thread safe.
    if (!SymFromAddr(process, *(DWORD64*)&self, &displacement, i))
    {
        return 0;
    }

    info->dli_fname = NULL;
    info->dli_fbase = NULL;
    info->dli_sname = i->Name;
    info->dli_saddr = address;

    return 1;
}
#endif
