#include "avium/core.h"

#include "avium/private/thread-context.h"
#include "avium/private/virtual.h"

#include "avium/string.h"
#include "avium/testing.h"
#include "avium/thread.h"
#include "avium/typeinfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bdwgc/include/gc.h>

#ifdef AVM_HAVE_BACKTRACE
#include <execinfo.h>
#endif

#define _ AvmRuntimeGetResource

//
// Object functions.
//

object AvmObjectNew(const AvmType* type)
{
    pre
    {
        assert(type != NULL);
    }

    object o = AvmAlloc(AvmTypeGetSize(type));
    GC_register_finalizer(o,
                          (GC_finalization_proc)(AvmCallback)AvmObjectFinalize,
                          NULL,
                          NULL,
                          NULL);
    *(const AvmType**)o = type;

    return o;
}

const AvmType* AvmObjectGetType(object self)
{
    pre
    {
        assert(self != NULL);
    }

    // The first member of an object should be a const AvmType*
    // Look in types.h for AVM_CLASS
    return *(const AvmType**)self;
}

void AvmObjectSurpressFinalizer(object self)
{
    pre
    {
        assert(self != NULL);
    }

    GC_register_finalizer(self, NULL, NULL, NULL, NULL);
}

void* AvmObjectVisit(object self, const AvmMember* member)
{
    pre
    {
        assert(self != NULL);
        assert(member != NULL);
    }

    return ((byte*)self) + member->_offset;
}

//
// Virtual calls.
//

bool AvmObjectEquals(object self, object other)
{
    VIRTUAL_CALL(bool, FnEntryEquals, self, other);
}

void AvmObjectFinalize(object self)
{
    AvmObjectSurpressFinalizer(self);
    VIRTUAL_CALL(void, FnEntryFinalize, self);
}

object AvmObjectClone(object self)
{
    VIRTUAL_CALL(object, FnEntryClone, self);
}

AvmString AvmObjectToString(object self)
{
    VIRTUAL_CALL(AvmString, FnEntryToString, self);
}

//
// Boxing
//

AvmBox AvmRuntimeBoxInt(_long value)
{
    return (AvmBox){
        ._type = typeid(_long),
        .AsInt = value,
    };
}

AvmBox AvmRuntimeBoxFloat(double value)
{
    return (AvmBox){
        ._type = typeid(double),
        .AsFloat = value,
    };
}

AvmBox AvmRuntimeBoxUint(ulong value)
{
    return (AvmBox){
        ._type = typeid(ulong),
        .AsUint = value,
    };
}

AvmBox AvmRuntimeBoxStr(str value)
{
    pre
    {
        assert(value != NULL);
    }

    return (AvmBox){
        ._type = typeid(str),
        .AsStr = value,
    };
}

//
// Type info for primitive types.
//

static void objectFinalize(object self)
{
    pre
    {
        assert(self != NULL);
    }

    (void)self;
}

static bool objectEquals(object self, object other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    const size_t size = AvmTypeGetSize(AvmObjectGetType(self));
    return memcmp(self, other, size) == 0;
}

static object objectClone(object self)
{
    pre
    {
        assert(self != NULL);
    }

    const AvmType* type = AvmObjectGetType(self);
    object o = AvmObjectNew(type);
    AvmCopy(self, AvmTypeGetSize(type), (byte*)o);
    return o;
}

static AvmString objectToString(object self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(
        AVM_OBJECT_FMT_STR, AvmTypeGetName(AvmObjectGetType(self)), self);
}

static AvmString signedToString(const AvmBox* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromInt(self->AsInt);
}

static bool signedEquals(const AvmBox* self, const AvmBox* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return self->AsInt == other->AsInt;
}

static AvmString unsignedToString(const AvmBox* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromUint(self->AsUint, NumericBaseDecimal);
}

static bool unsignedEquals(const AvmBox* self, const AvmBox* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return self->AsUint == other->AsUint;
}

static AvmString floatToString(const AvmBox* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromFloat(self->AsFloat, FloatReprAuto);
}

static bool floatEquals(const AvmBox* self, const AvmBox* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return self->AsFloat == other->AsFloat;
}

static AvmString strToString(const AvmBox* self)
{
    pre
    {
        assert(self != NULL);
        assert(self->AsStr != NULL);
    }

    return AvmStringFrom(self->AsStr);
}

static bool strEquals(const AvmBox* self, const AvmBox* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
        assert(self->AsStr != NULL);
        assert(other->AsStr != NULL);
    }

    return strcmp(self->AsStr, other->AsStr) == 0;
}

// For consistency with other primitive types.
static AvmString voidToString()
{
    return AvmStringNew(0);
}

static bool voidEquals()
{
    return false;
}

AVM_TYPE(object,
         object,
         {
             [FnEntryFinalize] = (AvmCallback)objectFinalize,
             [FnEntryEquals] = (AvmCallback)objectEquals,
             [FnEntryClone] = (AvmCallback)objectClone,
             [FnEntryToString] = (AvmCallback)objectToString,
         });

AVM_TYPE(_long,
         object,
         {
             [FnEntryToString] = (AvmCallback)signedToString,
             [FnEntryEquals] = (AvmCallback)signedEquals,
         });

AVM_TYPE(ulong,
         object,
         {
             [FnEntryToString] = (AvmCallback)unsignedToString,
             [FnEntryEquals] = (AvmCallback)unsignedEquals,
         });

AVM_TYPE(int,
         object,
         {
             [FnEntryToString] = (AvmCallback)signedToString,
             [FnEntryEquals] = (AvmCallback)signedEquals,
         });

AVM_TYPE(uint,
         object,
         {
             [FnEntryToString] = (AvmCallback)unsignedToString,
             [FnEntryEquals] = (AvmCallback)unsignedEquals,
         });

AVM_TYPE(short,
         object,
         {
             [FnEntryToString] = (AvmCallback)signedToString,
             [FnEntryEquals] = (AvmCallback)signedEquals,
         });

AVM_TYPE(ushort,
         object,
         {
             [FnEntryToString] = (AvmCallback)unsignedToString,
             [FnEntryEquals] = (AvmCallback)unsignedEquals,
         });

AVM_TYPE(char,
         object,
         {
             [FnEntryToString] = (AvmCallback)signedToString,
             [FnEntryEquals] = (AvmCallback)signedEquals,
         });

AVM_TYPE(byte,
         object,
         {
             [FnEntryToString] = (AvmCallback)unsignedToString,
             [FnEntryEquals] = (AvmCallback)unsignedEquals,
         });

AVM_TYPE(str,
         object,
         {
             [FnEntryToString] = (AvmCallback)strToString,
             [FnEntryEquals] = (AvmCallback)strEquals,
         });

AVM_TYPE(float,
         object,
         {
             [FnEntryToString] = (AvmCallback)floatToString,
             [FnEntryEquals] = (AvmCallback)floatEquals,
         });

AVM_TYPE(double,
         object,
         {
             [FnEntryToString] = (AvmCallback)floatToString,
             [FnEntryEquals] = (AvmCallback)floatEquals,
         });

// Because we can't sizeof(void) we have to do this manually.
static AvmCallback AVM_VT_NAME(void)[] = {
    [FnEntryToString] = (AvmCallback)voidToString,
    [FnEntryEquals] = (AvmCallback)voidEquals,
};

const AvmType AVM_TI_NAME(void) = {
    ._type = typeid(AvmType),
    ._vPtr = AVM_VT_NAME(void),
    ._name = "void",
    ._baseType = typeid(object),
    ._vSize = sizeof(AVM_VT_NAME(void)),
    ._size = 0,
};

//
// AvmLocation type.
//

static AvmString AvmLocationToString(AvmLocation* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(AVM_LOCATION_FMT_STR, self->File, self->Line);
}

AVM_TYPE(AvmLocation,
         object,
         {[FnEntryToString] = (AvmCallback)AvmLocationToString});

AvmLocation AvmLocationFrom(str file, uint line, uint column)
{
    pre
    {
        assert(file != NULL);
    }

    return (AvmLocation){
        ._type = typeid(AvmLocation),
        .File = file,
        .Line = line,
        .Column = column,
    };
}

//
// AvmVersion type.
//

static AvmString AvmVersionToString(AvmVersion* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(
        AVM_VERSION_FMT_STR, self->Major, self->Minor, self->Patch);
}

AVM_TYPE(AvmVersion,
         object,
         {[FnEntryToString] = (AvmCallback)AvmVersionToString});

AvmVersion AvmVersionFrom(ushort major, ushort minor, ushort patch)
{
    return (AvmVersion){
        ._type = typeid(AvmVersion),
        .Major = major,
        .Minor = minor,
        .Patch = patch,
    };
}

//
// Builtin runtime functions.
//

static AvmString AvmRuntimeToString(const AvmRuntime* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(AVM_RUNTIME_FMT_STR, self->_name, &self->_version);
}

AVM_TYPE(AvmRuntime,
         object,
         {
             [FnEntryToString] = (AvmCallback)AvmRuntimeToString,
         });

static AvmRuntime __AvmRuntimeState;

AvmExitCode AvmRuntimeInit(int argc, str argv[], AvmEntryPoint entry)
{
    GC_INIT();

    __AvmRuntimeState._type = typeid(AvmRuntime);
    __AvmRuntimeState._argc = (uint)(argc - 1);
    __AvmRuntimeState._argv = argv + 1;
    __AvmRuntimeState._name = AVM_RUNTIME_NAME;
    __AvmRuntimeState._version =
        AvmVersionFrom(AVM_VERSION_MAJOR, AVM_VERSION_MINOR, AVM_VERSION_PATCH);

    return __AvmRuntimeThreadInit(
        AvmThreadContextNew(NULL, (AvmThreadEntryPoint)entry));
}

str AvmRuntimeGetProgramName(void)
{
    return __AvmRuntimeState._argv[-1];
}

AvmVersion AvmRuntimeGetVersion(void)
{
    return __AvmRuntimeState._version;
}

uint AvmRuntimeGetArgCount(void)
{
    return __AvmRuntimeState._argc;
}

str* AvmRuntimeGetArgs(void)
{
    return __AvmRuntimeState._argv;
}

AvmString AvmRuntimeGetBacktrace(void)
{
#ifdef AVM_HAVE_BACKTRACE
    void* arr[AVM_BACKTRACE_MAX_SYMBOLS];

    int length = backtrace(arr, AVM_BACKTRACE_MAX_SYMBOLS);
    char** s = backtrace_symbols(arr, length);

    AvmString bt = AvmStringNew(length * AVM_BACKTRACE_LINE_SIZE);

    for (int i = length - 1; i >= 1; i--)
    {
        *(strchr(s[i], '+')) = '\0';
        *(strchr(s[i], '(')) = AVM_BACKTRACE_SEPARATOR;

        AvmStringPushStr(&bt, "    in ");
        AvmStringPushStr(&bt, s[i]);
        if (i != 1)
        {
            AvmStringPushChar(&bt, '\n');
        }
    }

    free(s);
    return bt;
#else
    return AvmStringFrom(BACKTRACE_NOT_AVAILABLE_MSG);
#endif
}

never AvmRuntimeThrow(object value, AvmLocation location)
{
    pre
    {
        assert(value != NULL);
    }

    const AvmThread* t = AvmThreadGetCurrent();

    t->_context->_location = location;
    t->_context->_thrownObject = value;
    longjmp(t->_context->_jumpBuffer, 1);
}

#define VA_LIST_TO_ARRAY_IMPL(T1, T2)                                          \
    for (size_t i = 0; i < length; i++)                                        \
    {                                                                          \
        ((T1*)array)[i] = (T1)va_arg(args, T2);                                \
    }                                                                          \
                                                                               \
    return array;

void* __AvmRuntimeMarshalVaList(va_list args, uint size, uint length)
{
    pre
    {
        assert(args != NULL);
        assert(size != 0);
        assert(length != 0);
    }

    byte* array = AvmAlloc(length * size);

    switch (size)
    {
    case sizeof(byte):
        VA_LIST_TO_ARRAY_IMPL(byte, uint);
    case sizeof(ushort):
        VA_LIST_TO_ARRAY_IMPL(ushort, uint);
    case sizeof(uint):
        VA_LIST_TO_ARRAY_IMPL(uint, uint);
    case sizeof(ulong):
        VA_LIST_TO_ARRAY_IMPL(ulong, ulong);
    default:
        throw(AvmErrorNew(_(AvmMarshalErrorMsg)));
    }
}

AVM_ENUM_TYPE(AvmResourceKey,
              {
                  AVM_ENUM_MEMBER(AvmArgErrorMsg),
                  AVM_ENUM_MEMBER(AvmMemErrorMsg),
                  AVM_ENUM_MEMBER(AvmRangeErrorMsg),
                  AVM_ENUM_MEMBER(AvmMarshalErrorMsg),
                  AVM_ENUM_MEMBER(AvmMissingMemberErrorMsg),
                  AVM_ENUM_MEMBER(AvmMissingConstantErrorMsg),
                  AVM_ENUM_MEMBER(AvmMissingCallbackErrorMsg),
              });

str AvmRuntimeGetResource(AvmResourceKey key)
{
    switch (key)
    {
    case AvmArgErrorMsg:
        return "Received an invalid argument.";
    case AvmMemErrorMsg:
        return "The system run out of memory.";
    case AvmRangeErrorMsg:
        return "The provided index was out of range.";
    case AvmMarshalErrorMsg:
        return "Marshalling of non primitive types is not supported.";
    case AvmMissingMemberErrorMsg:
        return "The requested member was missing.";
    case AvmMissingConstantErrorMsg:
        return "The requested constant was missing.";
    case AvmMissingCallbackErrorMsg:
        return "The requested callback was missing.";
    default:
        return "";
    }
}

//
// Allocation functions.
//

void* AvmAlloc(size_t size)
{
    void* mem = GC_malloc(size);
    if (mem == NULL)
    {
        GC_abort_on_oom();
    }
    return mem;
}

void* AvmRealloc(void* memory, size_t size)
{
    return GC_realloc(memory, size);
}

//
// Fundamental IO.
//

static void AvmFputs(str format, va_list args, FILE* stream)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
        assert(stream != NULL);
    }

    AvmString temp = AvmStringFormatV(format, args);
    fwrite(AvmStringGetBuffer(&temp), sizeof(char), temp._length, stream);
}

void AvmVScanf(str format, va_list args)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
    }

    // TODO: Ideally we should have a custom implementation.
    vscanf(format, args);
}

void AvmVPrintf(str format, va_list args)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
    }

    AvmFputs(format, args, stdout);
}

void AvmVErrorf(str format, va_list args)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
    }

    AvmFputs(format, args, stderr);
}

// These just forward to the V- equivalents.

void AvmScanf(str format, ...)
{
    pre
    {
        assert(format != NULL);
    }

    va_list args;
    va_start(args, format);
    AvmVScanf(format, args);
    va_end(args);
}

void AvmPrintf(str format, ...)
{
    pre
    {
        assert(format != NULL);
    }

    va_list args;
    va_start(args, format);
    AvmVPrintf(format, args);
    va_end(args);
}

void AvmErrorf(str format, ...)
{
    pre
    {
        assert(format != NULL);
    }

    va_list args;
    va_start(args, format);
    AvmVErrorf(format, args);
    va_end(args);
}

//
// AvmNativeError
//

AVM_CLASS(AvmNativeError, object, { int _code; });

static AvmString AvmNativeErrorToString(const AvmNativeError* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFrom(strerror(self->_code));
}

AVM_TYPE(AvmNativeError,
         object,
         {
             [FnEntryToString] = (AvmCallback)AvmNativeErrorToString,
         });

AvmError* AvmErrorFromOSCode(int code)
{
    AvmNativeError* e = AvmObjectNew(typeid(AvmNativeError));
    e->_code = code;
    return e;
}

//
// AvmDetailedError
//

AVM_CLASS(AvmDetailedError, object, { str _message; });

static AvmString AvmDetailedErrorToString(const AvmDetailedError* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFrom(self->_message);
}

AVM_TYPE(AvmDetailedError,
         object,
         {
             [FnEntryToString] = (AvmCallback)AvmDetailedErrorToString,
         });

AvmError* AvmErrorNew(str message)
{
    pre
    {
        assert(message != NULL);
    }

    AvmDetailedError* e = AvmObjectNew(typeid(AvmDetailedError));
    e->_message = message;
    return e;
}

//
// Exception implementation.
//

AVM_TYPE(AvmThrowContext, object, {[FnEntryFinalize] = NULL});

AvmThrowContext* __AvmRuntimeGetThrowContext(void)
{
    return AvmThreadGetCurrent()->_context;
}

void __AvmRuntimePushThrowContext(AvmThrowContext* context)
{
    AvmThread* t = (AvmThread*)AvmThreadGetCurrent();

    context->_type = typeid(AvmThrowContext);
    context->_thrownObject = NULL;
    context->_prev = t->_context;

    pthread_mutex_lock(t->_lock);
    t->_context = context;
    pthread_mutex_unlock(t->_lock);
}

AvmThrowContext* __AvmRuntimePopThrowContext(void)
{
    AvmThread* t = (AvmThread*)AvmThreadGetCurrent();
    AvmThrowContext* retval = t->_context;

    pthread_mutex_lock(t->_lock);
    t->_context = retval->_prev;
    pthread_mutex_unlock(t->_lock);

    return retval;
}

void AvmCopy(object o, size_t size, byte* destination)
{
    pre
    {
        assert(o != NULL);
        assert(destination != NULL);
    }

    size_t objectSize = AvmTypeGetSize(AvmObjectGetType(o));

    if (objectSize > size)
    {
        // TODO: error
    }

    memcpy(destination, o, objectSize);
}

#ifdef AVM_INSERT_INIT_CODE
// This should be defined by user code.
extern void AvmMain();

int main(int argc, str argv[])
{
    return AvmRuntimeInit(argc, argv, AvmMain);
}
#endif
