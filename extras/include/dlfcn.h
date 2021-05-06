extern void* dlopen(const char* filename, int mode);
extern int dlclose(void* handle);
extern void* dlsym(void* handle, const char* name);
extern char* dlerror(void);

#define RTLD_LAZY         0
#define RTLD_NOW          0
#define RTLD_BINDING_MASK 0
#define RTLD_NOLOAD       0
#define RTLD_DEEPBIND     0
#define RTLD_GLOBAL       0
#define RTLD_LOCAL        0
#define RTLD_NODELETE     0
