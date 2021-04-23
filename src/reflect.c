#include "avium/reflect.h"

#ifdef AVM_HAVE_DLFCN_H
#    include "avium/runtime.h"
#    include "avium/resources.h"
#    include "avium/string.h"
#    include "avium/fmt.h"

#    include <stdio.h>
#    include <string.h>
#    include <dlfcn.h>

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

    AvmString command = AvmSprintf(CommandTemplate, path);
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

AVM_TYPE(AvmSharedObject, {[FnEntryDtor] = NULL});

AvmSharedObject AvmReflectLoadObject(str path) {
    if (path == NULL) {
        AvmPanic(PathNullMsg);
    }

    void* handle = dlopen(path, RTLD_LAZY);
    AvmArrayList(AvmString) symbols = GetSymbolList(path);

    if (handle == NULL) {
        AvmPanic(dlerror());
    }

    return (AvmSharedObject){
        ._type = typeid(AvmSharedObject),
        ._handle = handle,
        ._symbols = symbols,
    };
}

size_t AvmReflectGetSymbolCount(AvmSharedObject* self) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    return AvmArrayListGetLength(AvmString)(&self->_symbols);
}

AvmSymbolType AvmReflectGetSymbolType(AvmSharedObject* self, str name) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (name == NULL) {
        AvmPanic(NameNullMsg);
    }

    for (size_t i = 0; i < AvmReflectGetSymbolCount(self); i++) {
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

AvmType* AvmReflectGetType(AvmSharedObject* self, str name) {
    if (self == NULL) {
        AvmPanic(SelfNullMsg);
    }

    if (name == NULL) {
        AvmPanic(NameNullMsg);
    }

    AvmSymbolType type = AvmReflectGetSymbolType(self, name);

    if (type != SymbolTypeType) {
        AvmPanic(MissingSymbolMsg);
    }

    AvmString string = AvmSprintf("_TI_%s", name);
    AvmType* t = dlsym(self->_handle, AvmStringAsPtr(&string));
    AvmObjectDestroy(&string);
    return t;
}

AvmFunction AvmReflectGetFunction(AvmSharedObject* self, str name) {
    AvmSymbolType type = AvmReflectGetSymbolType(self, name);

    if (type != SymbolTypeFunction) {
        AvmPanic(MissingSymbolMsg);
    }

    return (AvmFunction)dlsym(self->_handle, name);
}

void* AvmReflectGetVariable(AvmSharedObject* self, str name) {
    AvmSymbolType type = AvmReflectGetSymbolType(self, name);

    if (type != SymbolTypeVariable) {
        AvmPanic(MissingSymbolMsg);
    }

    return dlsym(self->_handle, name);
}

#endif  // AVM_HAVE_DLFCN_H
