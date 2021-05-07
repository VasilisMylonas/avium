#include "avium/reflect.h"

#include "avium/runtime.h"
#include "avium/string.h"

#include "avium/private/resources.h"

#include <stdio.h>
#include <string.h>

#include "avium/basename.h"
#include "avium/dlfcn.h"

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
#ifdef AVM_WIN32
    FILE* out = _popen(AvmStringAsPtr(&command), "r");
#else
    FILE* out = popen(AvmStringAsPtr(&command), "r");
#endif
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

static void AvmModuleDestroy(AvmModule* self) {
    AvmDlclose(self->_handle);
    self->_handle = NULL;

    size_t length = AvmArrayListGetLength(AvmString)(&self->_symbols);

    for (size_t i = 0; i < length; i++) {
        AvmObjectDestroy(&self->_symbols._items[i]);
    }

    AvmObjectDestroy(&self->_symbols);
}

AVM_TYPE(AvmModule, object, {[FnEntryDtor] = (AvmFunction)AvmModuleDestroy});

AvmModule AvmModuleLoad(str path) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    void* handle = AvmDlopen(path, RTLD_LAZY);
    if (handle == NULL) {
        AvmPanic(AvmDlerror());
    }

    str name = strrchr(path, '/');

    if (name == NULL) {
        name = strrchr(path, '\\');
    }

    if (name == NULL) {
        name = path;
    }

    return (AvmModule){
        ._type = typeid(AvmModule),
        ._handle = handle,
        ._name = name,
        ._symbols = GetSymbolList(path),
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
    AvmType* t = AvmDlsym(self->_handle, AvmStringAsPtr(&string));
    AvmObjectDestroy(&string);
    return t;
}

bool AvmModuleHasSymbol(AvmModule* self, str name) {
    return AvmModuleGetSymbolType(self, name) != SymbolTypeUnknown;
}

AvmFunction AvmModuleGetFunction(AvmModule* self, str name) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (name == NULL) {
        AvmPanic(NameNullMsg);
    }

    AvmSymbolType type = AvmModuleGetSymbolType(self, name);

    if (type != SymbolTypeFunction) {
        AvmPanic(MissingSymbolMsg);
    }

    void* ptr = AvmDlsym(self->_handle, name);
    // This weird thing is needed because apparently ISO C forbids conversion
    // between void* and void(*)(void).
    return *((AvmFunction*)&ptr);
}

void* AvmModuleGetVariable(AvmModule* self, str name) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (name == NULL) {
        AvmPanic(NameNullMsg);
    }

    AvmSymbolType type = AvmModuleGetSymbolType(self, name);

    if (type != SymbolTypeVariable) {
        AvmPanic(MissingSymbolMsg);
    }

    return AvmDlsym(self->_handle, name);
}

object AvmReflectConstructType(AvmType* type) {
    object o = AvmAlloc(AvmTypeGetSize(type));
    *(AvmType**)o = type;
    return o;
}

AVM_TYPE(AvmEnum, object, {[FnEntryDtor] = NULL});

str AvmEnumGetName(AvmEnum* self) { return self->_name; }
size_t AvmEnumGetSize(AvmEnum* self) { return self->_size; }

bool AvmEnumIsDefined(AvmEnum* self, long value) {
    for (size_t i = 0; true; i++) {
        if (self->_members[i]._value == 0 && self->_members[i]._name == NULL) {
            break;
        }

        if (self->_members[i]._value == value) {
            return true;
        }
    }

    return false;
}

str AvmEnumGetNameOf(AvmEnum* self, long value) {
    for (size_t i = 0; true; i++) {
        if (self->_members[i]._value == 0 && self->_members[i]._name == NULL) {
            break;
        }

        if (self->_members[i]._value == value) {
            return self->_members[i]._name;
        }
    }

    // TODO
    AvmPanic("TODO");
}

long AvmEnumGetValueOf(AvmEnum* self, str name) {
    for (size_t i = 0; true; i++) {
        if (self->_members[i]._value == 0 && self->_members[i]._name == NULL) {
            break;
        }

        if (strcmp(self->_members[i]._name, name) == 0) {
            return self->_members[i]._value;
        }
    }

    // TODO
    AvmPanic("TODO");
}
