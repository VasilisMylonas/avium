extern void* dlopen(const char* filename, int mode);
extern int dlclose(void* handle);
extern void* dlsym(void* handle, const char* name);
extern char* dlerror(void);
