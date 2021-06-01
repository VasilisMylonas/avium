#include "avium/config.h"

#ifdef AVM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// This should come after windows.h
#include <dbghelp.h>
#else
#define _GNU_SOURCE
#include <dlfcn.h>
#endif

#include "avium/reflect.h"

#include "avium/dlfcn.h"
#include "avium/private/basename.h"
#include "avium/private/constants.h"
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
    AvmString library = AvmStringFormat(WIN32_LIB_STR, buffer);
#elif defined AVM_DARWIN
    AvmString library = AvmStringFormat(DARWIN_LIB_STR, buffer);
#else
    AvmString library = AvmStringFormat(LINUX_LIB_STR, buffer);
#endif

    AvmString symbol = AvmStringFormat(TYPE_SYMBOL_STR, &buffer[index + 1]);

    void* handle = dlopen(AvmStringGetBuffer(&library), RTLD_LAZY);
    const AvmType* t = dlsym(handle, AvmStringGetBuffer(&symbol));

    if (t == NULL)
    {
        throw(AvmErrorNew(TypeNotPresentError));
    }

    return t;
}

static AvmString AvmModuleToString(const AvmModule* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(MODULE_STR, self->_name, self->_handle);
}

static AvmModule* AvmModuleLoadImpl(str path)
{
    void* handle = dlopen(path, RTLD_LAZY);

    if (handle == NULL)
    {
        throw(AvmErrorNew(dlerror()));
    }

    AvmModule* mod = AvmObjectNew(typeid(AvmModule));
    mod->_handle = handle;
    mod->_name = path == NULL ? AvmBasename(AvmRuntimeGetProgramName())
                              : AvmBasename(path);
    return mod;
}

AVM_TYPE(AvmModule,
         object,
         {
             [FnEntryFinalize] = (AvmFunction)AvmModuleUnload,
             [FnEntryToString] = (AvmFunction)AvmModuleToString,
         });

AvmModule* AvmModuleLoad(str path)
{
    pre
    {
        assert(path != NULL);
    }

    return AvmModuleLoadImpl(path);
}

void AvmModuleUnload(AvmModule* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmObjectSurpressFinalizer((object)self);
    dlclose(self->_handle);
}

const AvmModule* AvmModuleGetCurrent(void)
{
    static bool isLoaded = false;
    static AvmModule* module;

    if (!isLoaded)
    {
        module = AvmModuleLoadImpl(NULL);
        isLoaded = true;
    }

    post
    {
        assert(module != NULL);
    }

    return module;
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

    AvmString symbolName = AvmStringFormat(TYPE_SYMBOL_STR, name);
    const AvmType* type = dlsym(self->_handle, AvmStringToStr(&symbolName));

    if (type == NULL)
    {
        throw(AvmErrorNew(TypeNotPresentError));
    }

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

    if (ptr == NULL)
    {
        throw(AvmErrorNew(SymbolNotPresentError));
    }

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

    void* ptr = dlsym(self->_handle, name);

    if (ptr == NULL)
    {
        throw(AvmErrorNew(SymbolNotPresentError));
    }

    return ptr;
}

static AvmFunctionInfo* AvmFunctionGetInfo(AvmFunction self)
{
    pre
    {
        assert(self != NULL);
    }

#ifdef AVM_WIN32
    DWORD64 displacement = 0;
    HANDLE process = GetCurrentProcess();
    if (!SymInitialize(process, NULL, FALSE))
    {
        throw(AvmErrorFromOSCode((int)GetLastError()));
    }

    PSYMBOL_INFO info = AvmAlloc(sizeof(SYMBOL_INFO) + MAX_SYM_NAME);
    info->SizeOfStruct = sizeof(SYMBOL_INFO);
    info->MaxNameLen = MAX_SYM_NAME;

    // TODO: Not thread safe.
    if (!SymFromAddr(process, *(DWORD64*)&self, &displacement, info))
    {
        throw(AvmErrorFromOSCode((int)GetLastError()));
    }

    AvmString symbol = AvmStringFormat(TYPE_SYMBOL_STR, info->Name);
#else
    Dl_info info;
    dladdr(*(void**)&self, &info);

    AvmString symbol = AvmStringFormat(TYPE_SYMBOL_STR, info.dli_sname);
#endif

    return AvmModuleGetVariable(AvmModuleGetCurrent(), AvmStringToStr(&symbol));
}

static AvmString AvmFunctionInfoToString(const AvmFunctionInfo* self)
{
    pre
    {
        assert(self != NULL);
    }

    AvmString s = AvmStringNew(self->_paramCount * 4);
    AvmStringPushStr(&s, "function ");
    AvmStringPushStr(&s, self->_name);
    AvmStringPushStr(&s, " (");
    AvmStringPushUint(&s, self->_paramCount, NumericBaseDecimal);
    if (self->_paramCount == 1)
    {
        AvmStringPushStr(&s, " parameter)");
    }
    else
    {
        AvmStringPushStr(&s, " parameters)");
    }
    return s;
}

AVM_TYPE(AvmFunctionInfo,
         object,
         {
             [FnEntryToString] = (AvmFunction)AvmFunctionInfoToString,
         });

const AvmType* AvmFunctionGetReturnType(AvmFunction self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmFunctionGetInfo(self)->_returnType;
}

const AvmType** AvmFunctionGetParams(AvmFunction self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmFunctionGetInfo(self)->_paramTypes;
}

uint AvmFunctionGetParamCount(AvmFunction self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmFunctionGetInfo(self)->_paramCount;
}

str AvmFunctionGetName(AvmFunction self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmFunctionGetInfo(self)->_name;
}
