#pragma warning(push)
#pragma warning(disable : 2059)
#pragma warning(disable : 2079)
#pragma warning(disable : 2371)
#pragma warning(disable : 4094)
#pragma warning(disable : 5103)
#include <windows.h>
#pragma warning(pop)

static __declspec(thread) DWORD last_dl_error;

void* dlopen(const char* filename, int mode) {
    (void)mode;

    if (filename == NULL) {
        return GetModuleHandleA(NULL);
    }

    void* handle = LoadLibraryA(filename);
    last_dl_error = GetLastError();
    return handle;
}

int dlclose(void* handle) {
    BOOL b = FreeLibrary(handle);
    last_dl_error = GetLastError();
    return !b;
}

void* dlsym(void* handle, const char* name) {
    void* addr = GetProcAddress(handle, name);
    last_dl_error = GetLastError();
    return addr;
}

char* dlerror(void) {
    char* message = NULL;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, last_dl_error,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), &message, 0, NULL);

    return message;
}
