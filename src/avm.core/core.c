#include "avium/core.h"

#include "avium/private/constants.h"
#include "avium/private/virtual.h"
#include "avium/string.h"
#include "avium/testing.h"
#include "avium/typeinfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gc.h"

#ifdef AVM_LINUX
#include <execinfo.h>
#endif

static void AvmRuntimeFinalize(object self, void* kind)
{
    if (kind == AVM_OBJECT)
    {
        AvmObjectFinalize(self);
    }
}

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
    GC_register_finalizer(o, AvmRuntimeFinalize, AVM_OBJECT, NULL, NULL);
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

    GC_register_finalizer(
        self, AvmRuntimeFinalize, ALREADY_FINALIZED, NULL, NULL);
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
// Default implementations.
//

static void AvmObjectFinalizeDefault(object self)
{
    pre
    {
        assert(self != NULL);
    }

    (void)self;
}

static bool AvmObjectEqualsDefault(object self, object other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    const size_t size = AvmTypeGetSize(AvmObjectGetType(self));
    return memcmp(self, other, size) == 0;
}

static object AvmObjectCloneDefault(object self)
{
    pre
    {
        assert(self != NULL);
    }

    uint size = AvmTypeGetSize(AvmObjectGetType(self));
    void* memory = AvmAlloc(size);
    AvmCopy(self, size, (byte*)memory);
    return memory;
}

static AvmString AvmObjectToStringDefault(object self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(
        OBJECT_STR, AvmTypeGetName(AvmObjectGetType(self)), self);
}

//
// Type info for primitive types.
//

AVM_TYPE(object,
         object,
         {
             [FnEntryFinalize] = (AvmFunction)AvmObjectFinalizeDefault,
             [FnEntryEquals] = (AvmFunction)AvmObjectEqualsDefault,
             [FnEntryClone] = (AvmFunction)AvmObjectCloneDefault,
             [FnEntryToString] = (AvmFunction)AvmObjectToStringDefault,
         });

static AvmString _longToString(const _long* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromInt(*self);
}

static bool _longEquals(const _long* self, const _long* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return *self == *other;
}

static AvmString intToString(const int* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromInt(*self);
}

static bool intEquals(const int* self, const int* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return *self == *other;
}

static AvmString shortToString(const int* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromInt(*self);
}

static bool shortEquals(const short* self, const short* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return *self == *other;
}

static AvmString charToString(const char* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromChars(1, self);
}

static bool charEquals(const char* self, const char* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return *self == *other;
}

static AvmString ulongToString(const ulong* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromUint(*self, NumericBaseDecimal);
}

static bool ulongEquals(const ulong* self, const ulong* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return *self == *other;
}

static AvmString uintToString(const uint* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromUint(*self, NumericBaseDecimal);
}

static bool uintEquals(const uint* self, const uint* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return *self == *other;
}

static AvmString ushortToString(const ushort* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromUint(*self, NumericBaseDecimal);
}

static bool ushortEquals(const ushort* self, const ushort* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return *self == *other;
}

static AvmString byteToString(const byte* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromUint(*self, NumericBaseDecimal);
}

static bool byteEquals(const byte* self, const byte* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return *self == *other;
}

static AvmString strToString(const str* self)
{
    pre
    {
        assert(self != NULL);
        assert(*self != NULL);
    }

    return AvmStringFrom(*self);
}

static bool strEquals(const str* self, const str* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
        assert(*self != NULL);
        assert(*other != NULL);
    }

    return strcmp(*self, *other) == 0;
}

static AvmString floatToString(const float* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromFloat(*self);
}

static bool floatEquals(const float* self, const float* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return *self == *other;
}

static AvmString doubleToString(const double* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromFloat(*self);
}

static bool doubleEquals(const double* self, const double* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return *self == *other;
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

AVM_TYPE(_long,
         object,
         {
             [FnEntryToString] = (AvmFunction)_longToString,
             [FnEntryEquals] = (AvmFunction)_longEquals,
         });

AVM_TYPE(ulong,
         object,
         {
             [FnEntryToString] = (AvmFunction)ulongToString,
             [FnEntryEquals] = (AvmFunction)ulongEquals,
         });

AVM_TYPE(int,
         object,
         {
             [FnEntryToString] = (AvmFunction)intToString,
             [FnEntryEquals] = (AvmFunction)intEquals,
         });

AVM_TYPE(uint,
         object,
         {
             [FnEntryToString] = (AvmFunction)uintToString,
             [FnEntryEquals] = (AvmFunction)uintEquals,
         });

AVM_TYPE(short,
         object,
         {
             [FnEntryToString] = (AvmFunction)shortToString,
             [FnEntryEquals] = (AvmFunction)shortEquals,
         });

AVM_TYPE(ushort,
         object,
         {
             [FnEntryToString] = (AvmFunction)ushortToString,
             [FnEntryEquals] = (AvmFunction)ushortEquals,
         });

AVM_TYPE(char,
         object,
         {
             [FnEntryToString] = (AvmFunction)charToString,
             [FnEntryEquals] = (AvmFunction)charEquals,
         });

AVM_TYPE(byte,
         object,
         {
             [FnEntryToString] = (AvmFunction)byteToString,
             [FnEntryEquals] = (AvmFunction)byteEquals,
         });

AVM_TYPE(str,
         object,
         {
             [FnEntryToString] = (AvmFunction)strToString,
             [FnEntryEquals] = (AvmFunction)strEquals,
         });

AVM_TYPE(float,
         object,
         {
             [FnEntryToString] = (AvmFunction)floatToString,
             [FnEntryEquals] = (AvmFunction)floatEquals,
         });

AVM_TYPE(double,
         object,
         {
             [FnEntryToString] = (AvmFunction)doubleToString,
             [FnEntryEquals] = (AvmFunction)doubleEquals,
         });

// Because we can't sizeof(void) we have to do this manually.
static AvmFunction AVM_VT_NAME(void)[] = {
    [FnEntryToString] = (AvmFunction)voidToString,
    [FnEntryEquals] = (AvmFunction)voidEquals,
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

    return AvmStringFormat(LOCATION_STR, self->File, self->Line);
}

AVM_TYPE(AvmLocation,
         object,
         {[FnEntryToString] = (AvmFunction)AvmLocationToString});

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

    return AvmStringFormat(VERSION_STR, self->Major, self->Minor, self->Patch);
}

AVM_TYPE(AvmVersion,
         object,
         {[FnEntryToString] = (AvmFunction)AvmVersionToString});

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

    return AvmStringFormat(RUNTIME_STR, self->_name, &self->_version);
}

AVM_TYPE(AvmRuntime,
         object,
         {
             [FnEntryToString] = (AvmFunction)AvmRuntimeToString,
         });

static thread_local AvmRuntime __AvmRuntimeState;

int AvmRuntimeInit(int argc, str argv[], AvmEntryPoint entry)
{
    __AvmRuntimeState._type = typeid(AvmRuntime);
    __AvmRuntimeState._argc = (uint)(argc - 1);
    __AvmRuntimeState._argv = argv + 1;
    __AvmRuntimeState._name = AVM_RUNTIME_NAME;
    __AvmRuntimeState._throwContext = NULL;
    __AvmRuntimeState._version =
        AvmVersionFrom(AVM_VERSION_MAJOR, AVM_VERSION_MINOR, AVM_VERSION_PATCH);

    AvmThrowContext context;
    __AvmRuntimePushThrowContext(&context);
    if (setjmp(context._jumpBuffer) == 0)
    {
        entry();
    }
    else if (instanceof (object, context._thrownObject))
    {
        object e = __AvmRuntimePopThrowContext();
        AvmErrorf(UNHANDLED_THROW_STR, e, e, &context._location);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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
#ifdef AVM_LINUX
    void* arr[BACKTRACE_MAX_SYMBOLS];

    int length = backtrace(arr, BACKTRACE_MAX_SYMBOLS);
    char** s = backtrace_symbols(arr, length);

    AvmString bt = AvmStringNew(length * BACKTRACE_LINE_SIZE);

    for (int i = length - 1; i >= 1; i--)
    {
        *(strchr(s[i], '+')) = '\0';
        *(strchr(s[i], '(')) = BACKTRACE_SEPARATOR;

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

    __AvmRuntimeState._throwContext->_location = location;
    __AvmRuntimeState._throwContext->_thrownObject = value;
    longjmp(__AvmRuntimeState._throwContext->_jumpBuffer, 1);
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
        throw(AvmErrorNew(MarshallingError));
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
    GC_register_finalizer(mem, AvmRuntimeFinalize, RAW_MEMORY, NULL, NULL);
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
             [FnEntryToString] = (AvmFunction)AvmNativeErrorToString,
         });

AvmError* AvmErrorFromOSCode(int code)
{
    AvmNativeError* e = AvmAlloc(sizeof(AvmNativeError));
    e->_type = typeid(AvmNativeError);
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
             [FnEntryToString] = (AvmFunction)AvmDetailedErrorToString,
         });

AvmError* AvmErrorNew(str message)
{
    pre
    {
        assert(message != NULL);
    }

    AvmDetailedError* e = AvmAlloc(sizeof(AvmDetailedError));
    e->_type = typeid(AvmDetailedError);
    e->_message = message;
    return e;
}

//
// Exception implementation.
//

AVM_TYPE(AvmThrowContext, object, {[FnEntryFinalize] = NULL});

AvmThrowContext* __AvmRuntimeGetThrowContext(void)
{
    return __AvmRuntimeState._throwContext;
}

void __AvmRuntimePushThrowContext(AvmThrowContext* context)
{
    context->_type = typeid(AvmThrowContext);
    context->_thrownObject = NULL;
    context->_prev = __AvmRuntimeState._throwContext;
    __AvmRuntimeState._throwContext = context;
}

AvmThrowContext* __AvmRuntimePopThrowContext(void)
{
    AvmThrowContext* retval = __AvmRuntimeState._throwContext;
    __AvmRuntimeState._throwContext = retval->_prev;
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
