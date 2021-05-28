#include "avium/reflect.h"

#include "avium/dlfcn.h"
#include "avium/private/basename.h"
#include "avium/string.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

const AvmType* AvmReflectLoadType(str name)
{
    pre
    {
        assert(name != NULL);
    }

    AvmString temp = AvmStringFrom(name);
    uint index = AvmStringIndexOf(&temp, '@');

    char* buffer = AvmStringGetBuffer(&temp);
    buffer[index] = '\0';

#if defined AVM_WIN32
    AvmString library = AvmStringFormat("%s.dll", buffer);
#elif defined AVM_DARWIN
    AvmString library = AvmStringFormat("lib%s.dylib", buffer);
#else
    AvmString library = AvmStringFormat("lib%s.so", buffer);
#endif

    AvmString symbol = AvmStringFormat("_TI_%s", &buffer[index + 1]);

    void* handle = dlopen(AvmStringGetBuffer(&library), RTLD_LAZY);
    const AvmType* t = dlsym(handle, AvmStringGetBuffer(&symbol));

    AvmObjectDestroy(&library);
    AvmObjectDestroy(&symbol);
    AvmObjectDestroy(&temp);

    return t;
}

static void AvmModuleDestroy(AvmModule* self)
{
    pre
    {
        assert(self != NULL);
    }

    dlclose(self->_handle);
}

AVM_TYPE(AvmModule, object, {[FnEntryDtor] = (AvmFunction)AvmModuleDestroy});

static AvmModule AvmModuleLoadImpl(str path)
{
    void* handle = dlopen(path, RTLD_LAZY);

    if (handle == NULL)
    {
        throw(AvmErrorNew(dlerror()));
    }

    return (AvmModule){
        ._type = typeid(AvmModule),
        ._handle = handle,
        ._name = path == NULL ? AvmBasename(AvmRuntimeGetProgramName())
                              : AvmBasename(path),
    };
}

AvmModule AvmModuleLoad(str path)
{
    pre
    {
        assert(path != NULL);
    }

    return AvmModuleLoadImpl(path);
}

const AvmModule* AvmModuleGetCurrent(void)
{
    static bool isLoaded = false;
    static AvmModule module;

    if (!isLoaded)
    {
        module = AvmModuleLoadImpl(NULL);
        isLoaded = true;
    }

    return &module;
}

str AvmModuleGetName(const AvmModule* self)
{
    pre
    {
        assert(self != NULL);
    }

    return self->_name;
}

bool AvmModuleHasSymbol(const AvmModule* self, str name)
{
    pre
    {
        assert(self != NULL);
        assert(name != NULL);
    }

    return dlsym(self->_handle, name) != NULL;
}

const AvmType* AvmModuleGetType(const AvmModule* self, str name)
{
    pre
    {
        assert(self != NULL);
        assert(name != NULL);
    }

    AvmString symbolName = AvmStringFormat("_TI_%s%c", name, '\0');
    const AvmType* type = dlsym(self->_handle, AvmStringGetBuffer(&symbolName));
    AvmObjectDestroy(&symbolName);

    return type;
}

AvmFunction AvmModuleGetFunction(const AvmModule* self, str name)
{
    pre
    {
        assert(self != NULL);
        assert(name != NULL);
    }

    void* ptr = dlsym(self->_handle, name);
    // This weird thing is needed because apparently ISO C forbids conversion
    // between void* and void(*)(void).
    return *((AvmFunction*)&ptr);
}

void* AvmModuleGetVariable(const AvmModule* self, str name)
{
    pre
    {
        assert(self != NULL);
        assert(name != NULL);
    }

    return dlsym(self->_handle, name);
}
