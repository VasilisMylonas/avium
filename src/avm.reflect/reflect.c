#include "avium/reflect.h"

#include "avium/runtime.h"
#include "avium/string.h"

#include "avium/private/resources.h"

#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

static const str CommandTemplate =
    "nm --defined-only -g %s | awk '!/^(_{2}|\\.|\\/|\\s*$|.*:$)/ { print $2 "
    "$3 }'";

static AvmString ReadString(FILE* handle, bool* hasFailed) {
    char buffer[128] = {0};

    if (fgets(buffer, 128, handle) != buffer) {
        *hasFailed = true;
        return AvmStringNew(0);
    }

    *hasFailed = false;
    return AvmStringFrom(buffer);
}

static AvmArrayList(AvmString) GetSymbolList(str path) {
    AvmArrayList(AvmString) functions = AvmArrayListNew(AvmString)(10);

    AvmString command = AvmStringFormat(CommandTemplate, path);
    FILE* out = popen(AvmStringAsPtr(&command), "r");
    AvmObjectDestroy(&command);

    bool hasFailed = false;
    AvmString string;

    string = ReadString(out, &hasFailed);

    if (!hasFailed) {
        string._length--;
        AvmArrayListPush(AvmString)(&functions, string);
    }

    while (!hasFailed) {
        string = ReadString(out, &hasFailed);

        if (!hasFailed) {
            string._length--;
            AvmArrayListPush(AvmString)(&functions, string);
        }
    };

    fclose(out);

    return functions;
}

AVM_TYPE(AvmModule, object, {[FnEntryDtor] = NULL});

AvmModule AvmModuleLoad(str path) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    void* handle = dlopen(path, RTLD_LAZY);
    AvmArrayList(AvmString) symbols = GetSymbolList(path);

    if (handle == NULL) {
        AvmPanic(dlerror());
    }

    return (AvmModule){
        ._type = typeid(AvmModule),
        ._handle = handle,
        ._name = path,  // TODO: basename or something to get just the name.
        ._symbols = symbols,
    };
}

AvmModule* AvmModuleGetCurrent(void) {
    static bool isLoaded = false;
    static AvmModule module;

    if (!isLoaded) {
        module = AvmModuleLoad(AvmRuntimeGetProgramName());
        isLoaded = true;
    }

    return &module;
}

str AvmModuleGetName(AvmModule* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return self->_name;
}

size_t AvmModuleGetSymbolCount(AvmModule* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return AvmArrayListGetLength(AvmString)(&self->_symbols);
}

AvmSymbolType AvmModuleGetSymbolType(AvmModule* self, str name) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (name == NULL) {
        AvmPanic(NameNullMsg);
    }

    for (size_t i = 0; i < AvmModuleGetSymbolCount(self); i++) {
        AvmString* string = &self->_symbols._items[i];
        size_t length = AvmStringGetLength(string);
        char* buffer = AvmStringAsPtr(string);

        if (AvmStringStartsWithChar(string, AVM_NM_FN_PREFIX)) {
            if (strncmp(&buffer[1], name, length - 1) == 0) {
                return SymbolTypeFunction;
            }
            continue;
        }

        if (AvmStringStartsWithStr(string, AVM_NM_CLASS_PREFIX)) {
            if (strncmp(&buffer[5], name, length - 5) == 0) {
                return SymbolTypeType;
            }
            continue;
        }

        if (AvmStringStartsWithChar(string, AVM_NM_VAR_PREFIX)) {
            if (strncmp(&buffer[1], name, length - 1) == 0) {
                return SymbolTypeVariable;
            }
            continue;
        }
    }

    return SymbolTypeUnknown;
}

AvmType* AvmModuleGetType(AvmModule* self, str name) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (name == NULL) {
        AvmPanic(NameNullMsg);
    }

    AvmSymbolType type = AvmModuleGetSymbolType(self, name);

    if (type != SymbolTypeType) {
        AvmPanic(MissingSymbolMsg);
    }

    AvmString string = AvmStringFormat("_TI_%s", name);
    AvmType* t = dlsym(self->_handle, AvmStringAsPtr(&string));
    AvmObjectDestroy(&string);
    return t;
}

bool AvmModuleHasSymbol(AvmModule* self, str name) {
    return AvmModuleGetSymbolType(self, name) != SymbolTypeUnknown;
}

AvmFunction AvmModuleGetFunction(AvmModule* self, str name) {
    AvmSymbolType type = AvmModuleGetSymbolType(self, name);

    if (type != SymbolTypeFunction) {
        AvmPanic(MissingSymbolMsg);
    }

    void* ptr = dlsym(self->_handle, name);
    // This weird thing is needed because apparently ISO C forbids conversion
    // between void* and void(*)(void).
    return *((AvmFunction*)&ptr);
}

void* AvmModuleGetVariable(AvmModule* self, str name) {
    AvmSymbolType type = AvmModuleGetSymbolType(self, name);

    if (type != SymbolTypeVariable) {
        AvmPanic(MissingSymbolMsg);
    }

    return dlsym(self->_handle, name);
}

object AvmReflectConstructType(AvmType* type) {
    object o = AvmAlloc(AvmTypeGetSize(type));
    *(AvmType**)o = type;
    return o;
}
